import tkinter as tk
from tkinter import filedialog, messagebox
import subprocess
import os

def select_input_file():
    filename = filedialog.askopenfilename(filetypes=[("Assembly Files", "*.asm"), ("All Files", "*.*")])
    input_entry.delete(0, tk.END)
    input_entry.insert(0, filename)

def select_output_file():
    filename = filedialog.asksaveasfilename(defaultextension=".bin", filetypes=[("Binary Files", "*.bin"), ("All Files", "*.*")])
    output_entry.delete(0, tk.END)
    output_entry.insert(0, filename)

def assemble_code():
    input_file = input_entry.get()
    output_file = output_entry.get()
    
    if not input_file or not output_file:
        messagebox.showerror("Error", "Please select both input and output files.")
        return
    
    command = f"./assembler {input_file} {output_file}"
    try:
        result = subprocess.run(command, shell=True, capture_output=True, text=True)
        
        if result.returncode == 0:
            messagebox.showinfo("Success", "Assembly completed successfully!")
            view_output()
        else:
            messagebox.showerror("Error", result.stderr)
        
    except Exception as e:
        messagebox.showerror("Error", str(e))

def view_output():
    binary_output = "binary_output.txt"
    hex_output = "output.hex"
    
    if os.path.exists(binary_output):
        with open(binary_output, "r") as file:
            binary_text.delete("1.0", tk.END)
            binary_text.insert(tk.END, file.read())
    
    if os.path.exists(hex_output):
        with open(hex_output, "r") as file:
            hex_text.delete("1.0", tk.END)
            hex_text.insert(tk.END, file.read())

# GUI Setup
root = tk.Tk()
root.title("Assembly to Binary and Hex Assembler")
root.geometry("600x500")

tk.Label(root, text="Select Input Assembly File:").pack()
input_entry = tk.Entry(root, width=50)
input_entry.pack()
tk.Button(root, text="Browse", command=select_input_file).pack()

tk.Label(root, text="Select Output Binary File:").pack()
output_entry = tk.Entry(root, width=50)
output_entry.pack()
tk.Button(root, text="Browse", command=select_output_file).pack()

tk.Button(root, text="Assemble", command=assemble_code, fg="black", bg="green").pack(pady=10)

tk.Label(root, text="Binary Output:").pack()
binary_text = tk.Text(root, height=10, width=80)
binary_text.pack()

tk.Label(root, text="Hexadecimal Output:").pack()
hex_text = tk.Text(root, height=10, width=80)
hex_text.pack()

tk.Button(root, text="Exit", command=root.quit, fg="Black", bg="red").pack(pady=10)
root.mainloop()
