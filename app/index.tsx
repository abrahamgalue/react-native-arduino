import React, { useState } from 'react'
import { Text, View, Alert, TouchableOpacity } from 'react-native'
import axios from 'axios'

const esp01Url = 'http://192.168.4.1'

export default function Index() {
  const [status, setStatus] = useState('Unknown')

  const sendCommand = async (command: string) => {
    try {
      const response = await axios.post(esp01Url, `command=${command}`, {
        headers: {
          'Content-Type': 'application/x-www-form-urlencoded',
        },
        timeout: 5000,
      })
      setStatus(response.data)
    } catch (error: unknown) {
      if (axios.isAxiosError(error)) {
        if (error.code === 'ECONNABORTED') {
          Alert.alert(
            'Error',
            'The request took too long. Please check your connection to the ESP-01.'
          )
        } else if (error.message.includes('Network Error')) {
          Alert.alert(
            'Error',
            'Could not connect to ESP-01. Make sure you are connected to the correct network.'
          )
        } else {
          console.error('Error:', error)
          Alert.alert(
            'Error',
            'There was a problem communicating with the ESP-01.'
          )
        }
      } else {
        console.error('Unexpected error:', error)
        Alert.alert('Error', 'An unexpected error occurred.')
      }
    }
  }

  return (
    <View className='flex-1 justify-center items-center bg-blue-200'>
      <Text className='font-bold text-xl'>LED Control</Text>
      <TouchableOpacity
        className='my-2 font-bold rounded-full w-16 h-16 bg-green-500 items-center justify-center'
        onPress={() => sendCommand('ON')}
      >
        <Text>ON</Text>
      </TouchableOpacity>
      <TouchableOpacity
        className='font-bold rounded-full w-16 h-16 bg-red-500 items-center justify-center'
        onPress={() => sendCommand('OFF')}
      >
        <Text>OFF</Text>
      </TouchableOpacity>
      <View className='py-4'>
        <TouchableOpacity
          className='bg-blue-400 rounded-md p-2'
          onPress={() => sendCommand('STATE')}
        >
          <Text className='text-center'> Check Status</Text>
        </TouchableOpacity>
      </View>
      <Text>LED Status: {status}</Text>
    </View>
  )
}
