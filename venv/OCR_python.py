from flask import Flask, request, jsonify
from PIL import Image
import pytesseract
import base64
from io import BytesIO
import os

app = Flask(__name__)

@app.route('/ocr', methods=['POST'])
def ocr():
    # Get the image data from the request
    data = request.json
    image_data = base64.b64decode(data['image'])
    
    # Convert the image data to an image
    image = Image.open(BytesIO(image_data))
    
    # Perform OCR using Tesseracty
    text = pytesseract.image_to_string(image)
    
    # Return the OCR result as a JSON response
    return jsonify({"text": text})

# Add a new route to handle the image upload directly
@app.route('/upload', methods=['POST'])
def upload_image():
    if 'file' not in request.files:
        return jsonify({"error": "No file part"}), 400
    file = request.files['file']
    if file.filename == '':
        return jsonify({"error": "No selected file"}), 400
    image_data = file.read()
    # Convert the image data to an image
    image = Image.open(BytesIO(image_data))
    
    # Perform OCR using Tesseract
    text = pytesseract.image_to_string(image)
    
    # Return the OCR result as a JSON response
    return jsonify({"text": text})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
