import serial

import paho.mqtt.client as mqtt

# Create a serial connection
ser = serial.Serial('COM3', 115200) 

# Callback function for when the client receives a CONNACK response from the server
def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT server with result code " + str(rc))
    # Subscribe to a topic
    client.subscribe("g4inTopic")

# Callback function for when a message is received from the server
def on_message(client, userdata, msg):
    print("Received message: " + str(msg.payload.decode()))
    # Send the received message over serial
    ser.write(msg.payload)

# Create a new MQTT client instance
client = mqtt.Client()

# Set the callback functions
client.on_connect = on_connect
client.on_message = on_message

# Connect to the MQTT server
client.connect("broker.emqx.io", 1883, 60)

# Start the MQTT client loop
client.loop_start()

# Keep the program running
while True:
    pass
