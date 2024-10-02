import os

fn main() {
	path := '../dump.bin'

	line := os.read_file(path) or {
		print('Ошибка чтения файла: $err')
		return
	}
	mut sum := 0
	start_index := 0x14648
	for i := 0x0; i < 0x6BE00; i++ {
		sum += line[start_index + i]
	}
	println(sum)
}

// 56306664