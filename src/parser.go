package main

import (
  "bufio"
  "fmt"
  "log"
  "os"
  "strconv"
  "strings"
)

// Bitmap хранит индекс символа и его битовые данные
type Bitmap struct {
  Index int
  Data  [24]uint16
}

// parseBitmapRow конвертирует строку битов в uint16
func parseBitmapRow(row string) (uint16, error) {
  value, err := strconv.ParseUint(row, 16, 16) // Чтение строки в шестнадцатичном формате
  if err != nil {
    return 0, err
  }
  return uint16(value), nil
}

// parseBDF парсит BDF файл и возвращает массив битмапов
func parseBDF(filePath string) ([]Bitmap, error) {
  var bitmaps []Bitmap
  current_symbol := 0

  file, err := os.Open(filePath)
  if err != nil {
    return nil, fmt.Errorf("ошибка чтения файла: %w", err)
  }
  defer file.Close()

  scanner := bufio.NewScanner(file)
  for scanner.Scan() && current_symbol < 0x100 {
    line := scanner.Text()
    if strings.HasPrefix(line, "ENCODING") {
      number, _ := strconv.Atoi(strings.Split(line, " ")[1])
      for ; current_symbol < number; current_symbol++ {
        empty := Bitmap{
          Index: current_symbol,
          Data:  [24]uint16{},
        }
        bitmaps = append(bitmaps, empty)
      }
      for ; ; scanner.Scan() {
        line = scanner.Text()
        if strings.HasPrefix(line, "BBX") {
          break
        }
      }
      numbers := strings.Split(line, " ")
      hoff, _ := strconv.Atoi(numbers[4])
      hoff += 4
      if hoff < 0 {
        log.Fatalln("hoff < 0")
      }
      count_nice, _ := strconv.Atoi(numbers[2])
      count_trash := 24 - count_nice - hoff
      var data []uint16

      for i := 0; i < count_trash; i++ {
        data = append(data, 0)
      }
      count_trash = min(0, count_trash)
      for ; ; scanner.Scan() {
        line = scanner.Text()
        if strings.HasPrefix(line, "BITMAP") {
          break
        }
      }

      for i := 0; i < count_nice+count_trash; i++ {
        scanner.Scan()
        line = scanner.Text()
        bits, _ := strconv.ParseUint(line, 0x10, 16)
        data = append(data, uint16(bits))
      }

      for i := 0; i < hoff; i++ {
        data = append(data, 0)
      }
      bitmaps = append(bitmaps, Bitmap{
        Index: current_symbol,
        Data:  [24]uint16(data),
      })
      current_symbol++
    }
  }

  if err := scanner.Err(); err != nil {
    return nil, fmt.Errorf("ошибка при чтении файла: %w", err)
  }

  return bitmaps, nil
}

func writeBitmapsToFile(bitmaps []Bitmap, filePath string) error {
  file, err := os.Create(filePath)
  if err != nil {
    return fmt.Errorf("ошибка создания файла: %w", err)
  }
  defer file.Close()

  // Запись заголовка
  _, err = file.WriteString(`unsigned char font[256][24] = {`)

  if err != nil {
    return err
  }

  // Запись массива символов
  for i, bmp := range bitmaps {
    _, err = file.WriteString(fmt.Sprintf("  {//%x", i))
    if err != nil {
      return err
    }
    for _, row := range bmp.Data {
      _, err = file.WriteString(fmt.Sprintf("\n    0b%s,", fmt.Sprintf("%016b", row)))
      if err != nil {
        return err
      }
    }
    _, err = file.WriteString("\n  },")
    if err != nil {
      return err
    }
  }
  return err
}
func main() {
  // Замените "jb.bdf" на путь к вашему BDF-файлу
  bdfFilePath := "jb18.bdf"
  outputFilePath := "src/font.h"

  // Парсинг и запись в файл
  bitmaps, err := parseBDF(bdfFilePath)
  if err != nil {
    fmt.Printf("Не удалось распарсить файл BDF: %v\n", err)
    return
  }
  err = writeBitmapsToFile(bitmaps, outputFilePath)
  if err != nil {
    fmt.Printf("Не удалось записать в font.h: %v\n", err)
    return
  }
  fmt.Println("Массив символов успешно сохранен в font.h")
}
