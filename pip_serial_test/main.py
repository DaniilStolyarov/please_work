import serial
import time
from image_process import image_to_rgb565_bytes
image_path = 'why.jpg'
pixels = image_to_rgb565_bytes(image_path)
print(pixels.count(0))
print(len(pixels))
# Задайте параметры порта
port = 'COM7'  # Замените на ваш виртуальный COM-порт
baudrate = 1152000  # Задайте скорость передачи данных, соответствующую вашей настройке
end_pixels = 480*320 * 2
chunk_size = 17000
num_chunks = len(pixels) // chunk_size + 1
cur_chunk = 0
# Открываем последовательный порт
with serial.Serial(port, baudrate, timeout=1) as ser:
    while True:
        # Читаем данные
        response = ser.readline().decode('ascii')  # Читаем строку и убираем пробелы
        if (len(response) == 0):
            continue
        print(f"\nstm32f103rb output | {time.strftime("%H:%M:%S", time.localtime(time.time()))}:\n{response}")
        if response.startswith('READY') or response.endswith('READY'):
            
            start = cur_chunk * chunk_size
            end = start + chunk_size
            if end > end_pixels:  # Если конец выходит за пределы массива
                end = end_pixels
            pixel_chunk = bytearray(pixels[start:end])
            # Теперь можно использовать pixel_chunk для передачи данных
            ser.write(pixel_chunk) 
            print('end write' + str(chunk_size))
            cur_chunk+=1
                

