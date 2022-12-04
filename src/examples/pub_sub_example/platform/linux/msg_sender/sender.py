import struct
import zmq
#from ipcqueue import posixmq

help_message = '\
  1 - SYS_SIG_TYPE_1\n\
  2 - SYS_SIG_TYPE_2\n\
  3 - SYS_SIG_TYPE_3\n\
  4 - SYS_SIG_TYPE_4\n\
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
      print('sending SYS_SIG_TYPE_1')
      sender.send_string('SYS_SIG_TYPE_1')
    case '2':
      print('sending SYS_SIG_TYPE_2')
      sender.send_string('SYS_SIG_TYPE_2')
    case '3':
      print('sending SYS_SIG_TYPE_3')
      sender.send_string('SYS_SIG_TYPE_3')
    case '4':
      print('sending SYS_SIG_TYPE_4')
      sender.send_string('SYS_SIG_TYPE_4')
    case 'q':
      pass
    case _:
      print(help_message)