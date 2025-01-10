<?php
// Path to the image you want to upload
$imagePath = 'D:\github\iot_OCR\ocr.png';

// Initialize cURL
$ch = curl_init();

// Set the URL of the Flask upload endpoint
curl_setopt($ch, CURLOPT_URL, "http://localhost:5000/upload");

// Specify that this is a POST request
curl_setopt($ch, CURLOPT_POST, 1);

// Attach the image file to the request
curl_setopt($ch, CURLOPT_POSTFIELDS, [
    'file' => new CURLFile($imagePath)
]);

// Set options to return the response as a string
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

// Execute the request
$response = curl_exec($ch);

// Check for errors
if (curl_errno($ch)) {
    echo 'Error:' . curl_error($ch);
} else {
    // Print the response from the Flask server
    echo $response;
}

// Close the cURL session
curl_close($ch);
?>