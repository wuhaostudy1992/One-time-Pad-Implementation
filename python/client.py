from socket import socket, AF_INET, SOCK_STREAM
import sys
import simplejson

'''
harris@ics.uci.edu  Lan G. Harris
joe@cnn.com         Joe Smith
jane@slashdot.org   Jane Smith
'''

'''
mapping dictionary
'''
def dictionaryRead(mapPath):
    dictionary = {}
    with open(mapPath, 'rt') as f:
        for line in f:
            content = line.split(' ')
            dictionary[content[0]] = int(content[1])
    return dictionary

'''
process of ciphering message
'''
def cipher(dictionary, message, keyPath):
    key = []
    with open(keyPath, 'rt') as f:
        for line in f:
            content = list(line.strip('\n'))
            #print(content)
            for item in content:
                key.append(int(item))
    ciphertext = []
    i = 0
    #print(dictionary)
    #print(key)
    for c in message:
        ciphertext.append(dictionary[c] ^ key[i])
        i += 1
    return ciphertext


if __name__ == '__main__':
    dictionary = dictionaryRead('map.txt')
    if (len(sys.argv) < 2):
        print("Parameter is not completed")
        sys.exit()
    else:
        while True:
            message = input("Please enter the email you want to search:\n").replace(' ','')
            #print(message)
            ciphertext = cipher(dictionary, message, sys.argv[2])
            
            s = socket(AF_INET, SOCK_STREAM)
            try:  
                s.connect(('localhost', int(sys.argv[1])))
            except:
                print("connect error")
                sys.exit()
            
            st = simplejson.dumps(ciphertext)
            s.send(st.encode('utf-8'))

            print(str(s.recv(8192)))
