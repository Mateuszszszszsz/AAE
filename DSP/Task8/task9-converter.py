from PIL import Image
import numpy as np


def convert_to_grayscale(input_file, output_file):
    try:
        # Open the input BMP file
        with Image.open(input_file) as img:
            # Convert the image to grayscale
            grayscale_img = img.convert("L")
            grayscale_img = grayscale_img.resize((28, 28))  # Resize to 28x28
            #img_array = np.array(img).astype('float32') / 255.0  # Normalize
            #img_array = img_array.reshape(1, 28, 28, 1)  # Reshape for the model
            
            # Save the grayscale image as a new BMP file
            grayscale_img.save(output_file, format="PNG")
            print(f"Grayscale image saved as {output_file}")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    # Specify the input and output file paths
    input_file = "4.bmp"  # Replace with your input BMP file path
    output_file = "o4.png"  # Replace with your desired output BMP file path

    # Convert the input BMP file to grayscale and save it
    convert_to_grayscale(input_file, output_file)


def preprocess_image(image_path):
    img = Image.open(image_path).convert('L')  # Convert to grayscale
    img = img.resize((28, 28))  # Resize to 28x28
    img_array = np.array(img).astype('float32') / 255.0  # Normalize
    img_array = img_array.reshape(1, 28, 28, 1)  # Reshape for the model
    return img_array
