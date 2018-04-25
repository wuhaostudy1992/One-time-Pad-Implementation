from socketserver import BaseRequestHandler, TCPServer
import simplejson


'''
harris@ics.uci.edu  Lan G. Harris
joe@cnn.com         Joe Smith
jane@slashdot.org   Jane Smith
'''
dic = {'harris@ics.uci.edu':'Lan G. Harris',
       'joe@cnn.com'       :'Joe Smith',
       'jane@slashdot.org' :'Jane Smith',
       'test'              :'this is for testing'}


class EchoHandler(BaseRequestHandler):
    '''
    mapping dictionary
    '''
    def dictionaryRead(self, mapPath):
        dictionary = {}
        with open(mapPath, 'rt') as f:
            for line in f:
                content = line.split(' ')
                dictionary[content[1].strip()] = content[0]
        return dictionary

    '''
    process of deciphering message
    '''
    def decipher(self, dictionary, text, keyPath):
        result=''
        key = []
        with open(keyPath, 'rt') as f:
            for line in f:
                content = list(line.strip('\n'))
                for item in content:
                    key.append(int(item))
        
        for i in range(len(text)):
            result += dictionary[str(text[i] ^ key[i])]
        return result
        
    def handle(self):
        print('Address server started')
        #print('Got connection from', self.client_address)
        dictionary = self.dictionaryRead('map.txt')
        while True:
            msg = self.request.recv(8192)
            if not msg:
                break
            text = simplejson.loads(msg)
            result = self.decipher(dictionary, text, 'mykey.txt')
            print(result)
            if result in dic:
                self.request.send(dic[result].encode('utf-8'))
            else:
                self.request.send(b'unknown')

if __name__ == '__main__':
    serv = TCPServer(('', 20000), EchoHandler)
    serv.serve_forever()
