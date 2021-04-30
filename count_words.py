""" 
O arquivo usado para teste foi o t.txt com 9 linhas e 389 palavras
"""


file_name = input('Digite o nome do arquivo: ')

f = open(file_name, 'r')

linhas = len(f.readlines())
print("linhas: " + str(linhas))

f.seek(0, 0)

word_count = 0
words = []
for lines in f.readlines():
    linha = lines.rstrip()
    
    linhaEmLista = linha.split(' ')

    if(linhaEmLista[0] != ''):
        for i in linhaEmLista:
            words.append(i)
    
print("Palavras: " + str(len(words)))