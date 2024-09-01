from flask import Flask, request, make_response

app = Flask(__name__)

@app.route('/echo', methods=['POST'])
def echo_post():
    # Retrieve the binary data sent in the POST request
    binary_data = request.get_data()
    
    # Convert binary data to a hex string with spaces between bytes
    hex_string = ' '.join(f'{byte:02x}' for byte in binary_data)
    
    # Print the hex string
    print(hex_string)

    # Create a response object with the binary data
    response = make_response(binary_data)
    
    # Set the content type to 'application/octet-stream' for binary data
    response.headers.set('Content-Type', 'application/octet-stream')
    
    return response

@app.route('/echo/<data>', methods=['GET'])
def echo_get(data):
    # Create a response object with the data value
    response = make_response(data)
    
    # You can set the content type to 'text/plain' for textual data
    response.headers.set('Content-Type', 'text/plain')
    
    return response

if __name__ == '__main__':
    app.run(host = '0.0.0.0', debug=True)