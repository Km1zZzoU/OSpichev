package main

import (
	"fmt"
	"image/jpeg"
	"os"
)

func main() {
	file, err := os.Open("inter.jpeg")
	if err != nil {
		fmt.Println("Error: unable to open image file:", err)
		return
	}
	defer file.Close()

	img, err := jpeg.Decode(file)
	if err != nil {
		fmt.Println("Error: unable to decode JPEG file:", err)
		return
	}

	width := img.Bounds().Dx()
	height := img.Bounds().Dy()

	pixels := make([][]uint32, height)
	for y := range pixels {
		pixels[y] = make([]uint32, width)
	}

	for y := 0; y < height; y++ {
		for x := 0; x < width; x++ {
			pixelColor := img.At(x, y)
			r, g, b, a := pixelColor.RGBA()
			pixels[y][x] = (uint32(r>>8) << 24) | (uint32(g>>8) << 16) | (uint32(b>>8) << 8) | uint32(a>>8)
		}
	}

	writeToFile("src/buff.h", pixels)
}

func writeToFile(filename string, pixels [][]uint32) {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Println("Error: unable to create file:", err)
		return
	}
	defer file.Close()

	file.WriteString("u32 error = {\n")
	for _, row := range pixels {
		file.WriteString("    {")
		for i, pixel := range row {
			if i > 0 {
				file.WriteString(", ")
			}
			file.WriteString(fmt.Sprintf("0x%08x", pixel))
		}
		file.WriteString("},\n")
	}
	file.WriteString("};\n")
}
