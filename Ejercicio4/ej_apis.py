import requests
from flask import Flask, render_template, request

app = Flask(__name__)

def obtener_informacion_ubicacion(lugar):
    geonames_username = "chuy9211"
    url = f"http://api.geonames.org/searchJSON?name={lugar}&maxRows=1&username={geonames_username}"

    try:
        response = requests.get(url)
        data = response.json()
        if "geonames" in data and data["geonames"]:
            ubicacion = data["geonames"][0]
            datos_de_ubicacion = {"nombre": ubicacion['name'], "pais": ubicacion['countryName'], "codigo_pais": ubicacion['countryCode']}
        else:
            datos_de_ubicacion = {"error": "Ubicación no encontrada."}
    except Exception as e:
        datos_de_ubicacion = {"error": f"Error: {str(e)}"}
    return datos_de_ubicacion

def obtener_datos_meteorologicos(ciudad):
    datos_de_clima = {}
    api_key = "c617c1bfa1d4e7d3b44d27910a36b164"
    url = f"http://api.openweathermap.org/data/2.5/weather?q={ciudad}&appid={api_key}"

    try:
        response = requests.get(url)
        data = response.json()
        if "main" in data and "weather" in data:
            temperatura = data["main"]["temp"] - 273.15  # Convertir de Kelvin a Celsius
            condiciones_climaticas = data["weather"][0]["description"]
            datos_de_clima = {"temperatura": temperatura, "condiciones_climaticas": condiciones_climaticas}
        else:
            datos_de_clima = {"error": "Datos meteorológicos no disponibles."}
    except Exception as e:
        datos_de_clima = {"error": f"Error: {str(e)}"}
    return datos_de_clima

@app.route('/', methods=['GET', 'POST'])
def index():
    data = {}
    if request.method == 'POST':
        lugar = request.form['lugar']
        ubicacion_data = obtener_informacion_ubicacion(lugar)
        if 'error' in ubicacion_data:
            data['error'] = ubicacion_data['error']
        else:
            clima_data = obtener_datos_meteorologicos(ubicacion_data['nombre'])
            if 'error' in clima_data:
                data['error'] = clima_data['error']
            else:
                data = {'lugar': ubicacion_data['nombre'], 'pais': ubicacion_data['pais'], 'codigo_pais': ubicacion_data['codigo_pais'], 
                        'temperatura': clima_data['temperatura'], 'condiciones_climaticas': clima_data['condiciones_climaticas']}
    return render_template('index.html', data=data)

if __name__ == '__main__':
    app.run(debug=True)
