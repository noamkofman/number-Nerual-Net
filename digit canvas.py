import tkinter as tk
from PIL import Image, ImageDraw
import numpy as np
import numpy as np
import pandas as pd

WIDTH = 400
HEIGHT = 400
BRUSH = 12
weights = np.load("weights.npy")
bias = np.load("bias.npy")
root = tk.Tk()
root.title("Digit Drawer")

canvas = tk.Canvas(root, width=WIDTH, height=HEIGHT, bg="white")
canvas.pack()
step_size = 14
for x in range(0, WIDTH, step_size):
        canvas.create_line([(x, 0), (x, HEIGHT)], fill='#7f7f7f', tags='grid_line')
    
    # Create horizontal lines
for y in range(0, HEIGHT, step_size):
    canvas.create_line([(0, y), (WIDTH, y)], fill='#7f7f7f', tags='grid_line')

last = {"x": None, "y": None}

# in-memory image
buffer_img = Image.new("L", (WIDTH, HEIGHT), "white")
buffer_draw = ImageDraw.Draw(buffer_img)


def start(event):
    last["x"] = event.x
    last["y"] = event.y


def draw(event):
    if last["x"] is None:
        last["x"] = event.x
        last["y"] = event.y
        return

    canvas.create_line(
        last["x"], last["y"], event.x, event.y,
        width=BRUSH, fill="black",
        capstyle=tk.ROUND, smooth=True
    )

    buffer_draw.line(
        [last["x"], last["y"], event.x, event.y],
        fill="black",
        width=BRUSH
    )

    last["x"] = event.x
    last["y"] = event.y


def end(event):
    last["x"] = None
    last["y"] = None


def clear():
    canvas.delete("all")
    canvas.delete("pred_text")

    global buffer_img, buffer_draw
    buffer_img = Image.new("L", (WIDTH, HEIGHT), "white")
    buffer_draw = ImageDraw.Draw(buffer_img)
    step_size = 14
    for x in range(0, WIDTH, step_size):
            canvas.create_line([(x, 0), (x, HEIGHT)], fill='#7f7f7f', tags='grid_line')
        
        # Create horizontal lines
    for y in range(0, HEIGHT, step_size):
        canvas.create_line([(0, y), (WIDTH, y)], fill='#7f7f7f', tags='grid_line')



# -------- convert drawing to neural net input --------

def get_digit_vector():
    # resize to MNIST size
    img = buffer_img.resize((28, 28))
    # convert to numpy
    arr = np.array(img)
    # invert colors (MNIST style)
    arr = 255 - arr
    # normalize
    arr = arr / 255.0
    # flatten to 784 vector
    vector = arr.flatten()
    return vector
def capture():
    digit_vector = get_digit_vector()
    #print("Vector length:", len(digit_vector))  # 784
    #print("First 20 values:", digit_vector[:20])
    score = weights @ digit_vector + bias
    prediction = np.argmax(score)
    print(prediction)
    

    # Create a Label widget
    canvas.create_text(
        WIDTH - 80, 30,
        text=f"Pred: {prediction}",
        fill="red",
        font=("Arial", 22, "bold"),
        tags="pred_text"
    )

# mouse bindings
canvas.bind("<Button-1>", start)
canvas.bind("<B1-Motion>", draw)
canvas.bind("<ButtonRelease-1>", end)


controls = tk.Frame(root)
controls.pack(pady=10)

tk.Button(controls, text="Clear", command=clear).pack(side="left", padx=5)
tk.Button(controls, text="Predict", command=capture).pack(side="left", padx=5)

root.mainloop()
