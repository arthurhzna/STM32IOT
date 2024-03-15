from flask import Flask, request, jsonify
import datetime

app = Flask(__name__)

@app.route('/receive_data', methods=['POST', 'GET'])
def receive_data():
    if request.method == 'POST':
        data_received = request.get_json()  # Menerima data JSON dari ESP32
        if data_received:
            timestamp_str = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            char_buffer = data_received.get('char_buffer')  # Mendapatkan char buffer dari data JSON
            # Lakukan sesuatu dengan char buffer, misalnya:
            print("Menerima char buffer:", char_buffer)
            
            # Simpan char_buffer ke dalam file data.txt
            with open('data.txt', 'a') as file:
                file.write(f"{char_buffer} {timestamp_str}\n")
            
            response = {'message': 'Data diterima dengan sukses'}
            return jsonify(response), 200
        else:
            response = {'error': 'Data tidak valid'}
            return jsonify(response), 400
    elif request.method == 'GET':
        # Tangani permintaan GET jika diperlukan
        return 'Metode GET diterima'

if __name__ == '__main__':
    app.run(debug=True)  # Jalankan server Flask
