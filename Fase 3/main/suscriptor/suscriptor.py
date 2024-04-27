

import paho.mqtt.client as mqtt
import pymysql
import json

db_host = "database-1.cjgkkukgar69.us-east-2.rds.amazonaws.com"
db_user = "admin"
db_password = "123admin"
db_name = "sensores"


# Callback function for when the client receives a CONNACK response from the server
def on_connect_iluminacion(client, userdata, flags, rc):
    print("Connected to MQTT server with result code " + str(rc))

    # Subscribe to a topic
    client.subscribe("g4Luz")

# Callback function for when a message is received from the server
def on_message_iluminacion(client, userdata, msg):
    print("Received message: " + str(msg.payload.decode()))
   
    #aca insertar en la base de datos
 
    dato = json.loads(msg.payload.decode())
    valor = float(dato["luz"])    
    insertar_en_db("iluminacion", valor)

def on_connect_proximidad(client, userdata, flags, rc):
    print("Connected to MQTT server with result code " + str(rc))
    # Subscribe to a topic
    client.subscribe("g4Movimiento")

# Callback function for when a message is received from the server
def on_message_proximidad(client, userdata, msg):
    print("Received message: " + str(msg.payload.decode()))
    
    #aca insertar en la base de datos
    dato = json.loads(msg.payload.decode())
    valor = float(dato["mov"])
    insertar_en_db("proximidad", valor)

def on_connect_aire(client, userdata, flags, rc):
    print("Connected to MQTT server with result code " + str(rc))
    # Subscribe to a topic
    client.subscribe("g4CO2")

# Callback function for when a message is received from the server
def on_message_aire(client, userdata, msg):
    print("Received message: " + str(msg.payload.decode()))
    
    #aca insertar en la base de datos
    dato = json.loads(msg.payload.decode())
    valor = float(dato["co2"])
    insertar_en_db("aire", valor)

def on_connect_temperatura(client, userdata, flags, rc):
    print("Connected to MQTT server with result code " + str(rc))
    # Subscribe to a topic
    client.subscribe("g4Temperatura")

# Callback function for when a message is received from the server
def on_message_temperatura(client, userdata, msg):
    print("Received message: " + str(msg.payload.decode()))
    
    #aca insertar en la base de datos
    dato = json.loads(msg.payload.decode())
    valor = float(dato["temperatura"])
    insertar_en_db("temperatura", valor)

def on_connect_humedad(client, userdata, flags, rc):
    print("Connected to MQTT server with result code " + str(rc))
    # Subscribe to a topic
    client.subscribe("g4Humedad")

# Callback function for when a message is received from the server
def on_message_humedad(client, userdata, msg):
    print("Received message: " + str(msg.payload.decode()))
    
    #aca insertar en la base de datos
    dato = json.loads(msg.payload.decode())
    valor = float(dato["humedad"])
    insertar_en_db("humedad", valor)

def on_connect_seguridad(client, userdata, flags, rc):
    print("Connected to MQTT server with result code " + str(rc))
    # Subscribe to a topic
    client.subscribe("g4Servo")

# Callback function for when a message is received from the server
def on_message_seguridad(client, userdata, msg):
    print("Received message: " + str(msg.payload.decode()))
    
    #aca insertar en la base de datos
    dato = json.loads(msg.payload.decode())
    valor = dato["puerta"]
    insertar_en_db("seguridad", valor)

def insertar_en_db(tabla, valor):

    print(tabla)
    print(valor)

    try:
        # Establecer la conexión a la base de datos
        conn = pymysql.connect(host=db_host, user=db_user, password=db_password, db=db_name)
        cur = conn.cursor()

        # Insertar el valor en la tabla correspondiente
        sql = f"INSERT INTO {tabla} (valor, tiempo) VALUES ({valor}, NOW())"
        cur.execute(sql)
        conn.commit()

        print(f"Valor {valor} insertado en la tabla {tabla}")

    except Exception as e:
        print(f"Error al insertar en la base de datos: {e}")

    finally:
        # Cerrar la conexión a la base de datos
        conn.close()

# Create a new MQTT client instance

clienteIlimunacion = mqtt.Client()
clienteProximidad = mqtt.Client()
clienteAire = mqtt.Client()
clienteTemperatura = mqtt.Client()
clienteHumedad = mqtt.Client()
clienteSeguridad = mqtt.Client()


# Set the callback functions
#iluminacion
clienteIlimunacion.on_connect = on_connect_iluminacion
clienteIlimunacion.on_message = on_message_iluminacion

#proximidad
clienteProximidad.on_connect = on_connect_proximidad
clienteProximidad.on_message = on_message_proximidad

#aire
clienteAire.on_connect = on_connect_aire
clienteAire.on_message = on_message_aire

#temperatura
clienteTemperatura.on_connect = on_connect_temperatura
clienteTemperatura.on_message = on_message_temperatura

#humedad
clienteHumedad.on_connect = on_connect_humedad
clienteHumedad.on_message = on_message_humedad

#seguridad
clienteSeguridad.on_connect = on_connect_seguridad
clienteSeguridad.on_message = on_message_seguridad

# Connect to the MQTT server
#iluminacion
clienteIlimunacion.connect("broker.emqx.io", 1883, 60)
#proximidad
clienteProximidad.connect("broker.emqx.io", 1883, 60)
#aire
clienteAire.connect("broker.emqx.io", 1883, 60)
#temperatura
clienteTemperatura.connect("broker.emqx.io", 1883, 60)
#humead
clienteHumedad.connect("broker.emqx.io", 1883, 60)
#seguridad
clienteSeguridad.connect("broker.emqx.io", 1883, 60)


# Start the MQTT client loop
#iluminacion
clienteIlimunacion.loop_start()
#proximidad
clienteProximidad.loop_start()
#aire
clienteAire.loop_start()
#temperatura
clienteTemperatura.loop_start()
#humead
clienteHumedad.loop_start()
#seguridad
clienteSeguridad.loop_start()


# Keep the program running
while True:
    pass