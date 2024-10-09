import os
import rand

fn main() {
	output_file := 'BimBimBamBam.bin'
	file_size := 0x6BE00

	mut file := os.open_file(output_file, 'w') or {
		eprintln('Ошибка при открытии файла: $err')
		return
	}
	defer {
		file.close()
	}

	mut sum := 0

	for _ in 0 .. file_size {
		random_byte := rand.u8()
		sum += random_byte
		file.write([random_byte]) or {
			eprintln('Ошибка при записи в файл: $err')
			return
		}
	}

	println('Файл $output_file размером $file_size байт суммой $sum создан.')
}
