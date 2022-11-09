import struct
import zmq
#from ipcqueue import posixmq

help_message = '\
  1 - SYS_SIG_OPEN\n\
  2 - SYS_SIG_CLOSE\n\
  3 - SYS_SIG_TOAST\n\
  4 - SYS_SIG_BAKE\n\
  q - quit\
'
print('Signal sender, press key to send a signal')
print(help_message)

#queue = posixmq.Queue('/microwave')
#print('queue: ', queue)

ctx = zmq.Context()
sender = ctx.socket(zmq.PUSH)
sender.bind('tcp://*:5555')

key = None
while key != 'q':
  key = input('> ')
  match key:
    case '1':
      print('sending SYS_SIG_OPEN')
      sender.send_string('open')
    case '2':
      print('sending SYS_SIG_CLOSE')
      sender.send_string('close')
    case '3':
      print('sending SYS_SIG_TOAST')
      sender.send_string('toast')
    case '4':
      print('sending SYS_SIG_BAKE')
      sender.send_string('bake')
    case 'q':
      pass
    case _:
      print(help_message)