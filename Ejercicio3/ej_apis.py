import requests

def obtener_informacion_ubicacion(lugar):
    geonames_username = "chuy9211"
    url = f"http://api.geonames.org/searchJSON?name={lugar}&maxRows=1&username={geonames_username}"

    try:
        response = requests.get(url)
        data = response.json()
        if "geonames" in data and data["geonames"]:
            ubicacion = data["geonames"][0]
            datos_de_ubicacion = {ubicacion['countryName'],ubicacion['countryCode']}
            """ print(f"Nombre: {ubicacion['name']}")
            print(f"País: {ubicacion['countryName']}")
            print(f"Código de País: {ubicacion['countryCode']}") """
        else:
            print("Ubicación no encontrada.")
    except Exception as e:
        print(f"Error: {str(e)}")
    return datos_de_ubicacion

def obtener_datos_meteorologicos(ciudad):
    datos_de_clima = {}
    api_key="c617c1bfa1d4e7d3b44d27910a36b164"
    url = f"http://api.openweathermap.org/data/2.5/weather?q={ciudad}&appid={api_key}"

    try:
        response = requests.get(url)
        data = response.json()
        if "main" in data and "weather" in data:
            temperatura = data["main"]["temp"] - 273.15  # Convertir de Kelvin a Celsius
            condiciones_climaticas = data["weather"][0]["description"]
            datos_de_clima = {temperatura,condiciones_climaticas}
        else:
            print("Datos meteorológicos no disponibles.")
    except Exception as e:
        print(f"Error: {str(e)}")
    return datos_de_clima

if __name__ == "__main__":
    print("Ingrese el nombre del lugar que desea consultar: ")
    lugar = input()
    lcons = list(obtener_informacion_ubicacion(lugar))
    resu = list(obtener_datos_meteorologicos(lcons[0]))
    print(f"Temperatura en {lcons[0]}, {lcons[1]}: {resu[0]:.2f}°C")
    print(f"Condiciones climáticas en {lcons[0]}, {lcons[1]}: {resu[1]}")
