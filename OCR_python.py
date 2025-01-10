from flask import Flask, request, jsonify
from PIL import Image
import pytesseract
import base64
from io import BytesIO

app = Flask(__name__)

@app.route('/ocr', methods=['POST'])
def ocr():
    # Get the image data from the request
    data = request.json
    image_data = base64.b64decode(data['image'])
    
    # Convert the image data to an image
    image = Image.open(BytesIO(image_data))
    
    # Perform OCR using Tesseract
    text = pytesseract.image_to_string(image)
    
    # Return the OCR result as a JSON response
    return jsonify({"text": text})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
