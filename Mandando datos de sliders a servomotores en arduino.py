import serial
import time
from tkinter import *
import tkinter as tk

def encode_send(ser, texto):
    enc = f'{texto}\n'.encode('UTF-8')
    ser.write(enc)

def decode_response(ser):
    message_from_serial = ser.readline()
    res = message_from_serial[:-1].decode()
    return res

serialport = serial.Serial('COM5', 9600)
time.sleep(0.1)

class Window(Frame):

    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.master = master

        # Se crea menu principal
        main_menu = Menu(self.master)
        self.master.config(menu=main_menu)

        # Se crea menu secundario
        menu_config = Menu(main_menu)
        menu_config.add_command(label="Opcion 1")
        menu_config.add_command(label="Salir", command=self.exit_window)

        # Se agrega al menu principal
        main_menu.add_cascade(label="Opciones", menu=menu_config)

        # Creamos un contenedor para otros widgets
        self.pack(fill=tk.BOTH, expand=True)

        # Se agrega botón
        self.exitButton = Button(self, text="Cerrar", command=self.exit_window)
        self.exitButton.place(x=130, y=320)

        # Se agrega slider
        servo1 = self.create_scale("Servo-Motor 1", 0, 180)
        servo1.pack()
        servo2 = self.create_scale("Servo-Motor 2", 0, 180)
        servo2.pack()
        servo3 = self.create_scale("Servo-Motor 3", 0, 180)
        servo3.pack()
        
        

        def enviar_a_arduino():
            VA1 = servo1.get()
            VA2 = servo2.get()
            VA3 = servo3.get()

            mensaje = str(VA1) + ',' + str(VA2) + ',' + str(VA3) + '\n'
            encode_send(serialport, mensaje)
            print("Se manda el siguiente mensaje: ", mensaje)

        boton_enviar = tk.Button(self, text="Enviar", command=enviar_a_arduino)
        print(boton_enviar)
        boton_enviar.pack()

        

    

    # Método para crear la escala para cada slider
    def create_scale(self, label_text, from_, to):
        scale_label = tk.Label(self, bg='white', fg='black', width=35, text=label_text)
        scale_label.pack()

        def print_selection(v):
            scale_label.config(text=label_text + ': ' + v)

        return tk.Scale(self, label=label_text, from_=from_, to=to, orient=tk.HORIZONTAL, length=600,
                        showvalue=0, tickinterval=2, resolution=10, command=print_selection)


    # Método salir
    def exit_window(self):
        root.destroy()

# Inicializa la ventana
root = Tk()
app = Window(root)

# Agrega título
root.wm_title("Sliders")
root.geometry("320x400")

# Muestra la ventana
root.mainloop()

# Cierra el puerto serial al finalizar
serialport.close()
