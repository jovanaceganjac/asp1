import math as m
import os
import time

def formiraj_matricu():
    matrica = [[0, 0, 0], [0, 1, 2], [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3]]
    return matrica

def talon(matrica, csr):
    vrste = ['1', '2', '3', '4', '5', '6', 'Kenta', 'Ful', 'Poker', 'Jamb', '∑']
    kolone = ['', 'Na dole', 'Na gore', 'Rucna']
    sums = [0, 0, 0]
    for i in range(3):
        for j in range(6):
            try:
                sums[i] += ima(matrica, j, i)
            except TypeError:
                pass
    print('|{:^10}|{:^10}|{:^10}|{:^10}|'.format(*kolone))
    for i in range(11):
        print('|{:^10}|{:^10}|{:^10}|{:^10}|'.format(vrste[i], ima(matrica, i, 0), ima(matrica, i, 1), ima(matrica, i, 2)))
        if i == 5:
            suma1=sums[0]
            suma2=sums[1]
            suma3=sums[2]
            print('|{:^10}|{:^10}|{:^10}|{:^10}|'.format('∑', suma1, suma2, suma3))
        elif i == 9 or i == 10:
            print('')
def popuni(matrica, csr, vrsta_b, kolona_b, vrednost_b):
    if csr:
        for i in range(matrica[2][vrsta_b], matrica[2][vrsta_b+1]):
            if kolona_b < matrica[1][i]:
                matrica[0].insert(i, vrednost_b)
                matrica[1].insert(i, kolona_b)
                break
        else:
            matrica[0].insert(matrica[2][vrsta_b], vrednost_b)
            matrica[1].insert(matrica[2][vrsta_b], kolona_b)
        for i in range(vrsta_b+1, 12):
            matrica[2][i] += 1
    else:
        matrica[kolona_b][vrsta_b] = vrednost_b

def transformisi_matricu(matrica):
    nova_matrica = []
    for i in range(3):
        trenutno = []
        for j in range(11):
            trenutno.append(' ')
        nova_matrica.append(trenutno)
    for i in range(11):
        for j in range(matrica[2][i], matrica[2][i+1]):
            nova_matrica[j][i] = matrica[0][j]
    return nova_matrica

def ima(matrica, vrsta_b, kolona_b):
    global csr
    if csr:
        for i in range(matrica[2][vrsta_b], matrica[2][vrsta_b+1]):
            if matrica[1][i] == kolona_b:
                return matrica[0][i]
        return ' '
    else:
        return matrica[kolona_b][vrsta_b]

def popuni_polje(matrica, csr, vrsta_b, kolona_b, vrednost_b):
    if vrsta_b == 10:
        vrednost = ima(matrica, vrsta_b, kolona_b) + vrednost_b
        matrica[0][-3+kolona_b] = vrednost
    else:
        if csr:
            if len(matrica[0]) == 10:
                matrica = transformisi_matricu(matrica)
                csr = False
        popuni(matrica, csr, vrsta_b, kolona_b, vrednost_b)
    return csr

def BBS(x :int):
    sum = 0
    while (sum == 0 or sum == 7):
        sum = 0
        for i in range(3):
            x = (x * x) % 253
            sum = sum*2 + x % 2
    return (x, sum)

def prazna_polja(matrica):
    lista_praznih_polja = []
    for i in range(0, 10):
        if ima(matrica, i, 0) == ' ':
            lista_praznih_polja.append((i, 0))
            break
    for i in range(9, -1, -1):
        if ima(matrica, i, 1) == ' ':
            lista_praznih_polja.append((i, 1))
            break
    for i in range(0, 10):
        if ima(matrica, i, 2) == ' ':
            lista_praznih_polja.append((i, 2))
    return lista_praznih_polja

def nazivi_polja(lista_polja):
    kolone = ['Na dole', 'Na gore', 'Rucna']
    vrste = ['1', '2', '3', '4', '5', '6', 'kenta', 'ful', 'poker', 'jamb']
    imena_polja = []
    for polje in lista_polja:
        imena_polja.append((vrste[polje[0]], kolone[polje[1]]))
    return imena_polja

def izracunaj_vrednost(matrica, kockice, vrsta, kolona, bacanje):
    if bacanje == 2 and kolona == 2:
        return 0
    if vrsta == 0:
        return 1*kockice.count(1)
    elif vrsta == 1:
        return 2*kockice.count(2)
    elif vrsta == 2:
        return 3*kockice.count(3)
    elif vrsta == 3:
        return 4*kockice.count(4)
    elif vrsta == 4:
        return 5*kockice.count(5)
    elif vrsta == 5:
        return 6*kockice.count(6)
    elif vrsta == 6:
        if 1 in kockice and 2 in kockice and 3 in kockice and 4 in kockice and 5 in kockice:
            return 66-10*bacanje
        elif 2 in kockice and 3 in kockice and 4 in kockice and 5 in kockice and 6 in kockice:
            return 66-10*bacanje
    else:
        pojavljivanja = []
        for i in range(1, 7):
            pojavljivanja.append(kockice.count(i))
        if vrsta == 7:
            if 3 in pojavljivanja and 2 in pojavljivanja:
                return sum(kockice) + 30
        elif vrsta == 8:
            if 4 in pojavljivanja:
                return 4*(kockice.index(4)+1)
            elif 5 in pojavljivanja:
                return 4*(kockice.index(5)+1)
        elif vrsta == 9 and 5 in pojavljivanja:
            return sum(kockice)
    return 0

def odaberi_polje(matrica, csr, kockice, bacanje):
    prazna_polja_lista = prazna_polja(matrica)
    prazna_polja_nazivi = nazivi_polja(prazna_polja_lista)
    while True:
        print('\nOpcije:')
        for i in range(1, len(prazna_polja_lista)+1):
            print('{0}. {2} {1}'.format(i, *prazna_polja_nazivi[i-1]))
        while True:
            try:
                opcija = int(input())
                if opcija not in range(1, len(prazna_polja_lista)+1):
                    raise ValueError
                break
            except ValueError:
                print('\nOpcija koju ste odabrali ne postoji\nPokusajte ponovo...')
        vrednost = izracunaj_vrednost(matrica, kockice, prazna_polja_lista[opcija-1][0], prazna_polja_lista[opcija-1][1], bacanje)
        if vrednost == 0:
            print('\novo polje bice precrtano, nastavi?')
            pristanak = input().strip()
            if pristanak.lower() == 'ne':
                pass
        csr = popuni_polje(matrica, csr, prazna_polja_lista[opcija-1][0], prazna_polja_lista[opcija-1][1], vrednost)
        csr = popuni_polje(matrica, csr, 10, prazna_polja_lista[opcija - 1][1], vrednost)
        return csr

def baci_kockice(x, kockice :list, za_cuvanje):
    kockice_kopija = kockice.copy()
    for i in range(5):
        if i not in za_cuvanje:
            x, kockice_kopija[i] = BBS(x)
    return x, kockice_kopija

def odigraj_potez(matrica, x):
    bacanje = 0
    kockice = [0, 0, 0, 0, 0]
    za_cuvanje = []
    while True:
        x, kockice = baci_kockice(x, kockice, za_cuvanje)
        print('\nVase kocke\nKocka 1: {0}\nKocka 2: {1}\nKocka 3: {2}\nKocka 4: {3}\nKocka 5: {4}'.format(*kockice))

        if bacanje != 2:
            print('\nOpcije:\n1. Baci kockice\n2. Upisi potez\n\nOdaberi...')
            while True:
                try:
                    opcija = int(input())
                    if opcija not in range(1, 3):
                        raise ValueError
                    break
                except ValueError:
                    print('\nOpcija koju ste izabrali ne postoji\nPokusajte ponovo:')
        else:
            opcija = 2
        if opcija == 1:
            print('\nUnesite brojeve kockica koje biste da sacuvate: ')
            while True:
                try:
                    unos = [int(i) for i in input().split()]
                    for i in unos:
                        if i not in range(1, 6):
                            raise ValueError
                    for i in unos:
                        za_cuvanje.append(i-1)
                    bacanje += 1
                    break
                except ValueError:
                    print('\nKockica koju ste izabrali ne postoji\nPokusajte ponovo:')
        else:
            odaberi_polje(matrica, csr, kockice, bacanje)
            return x

matrica = formiraj_matricu()
while True:
    csr = True
    x = int(time.time())
    print('\nOpcije:\n1. Restartuj igru\n2. Prikazi talon\n3. Baci kockice\n4. Pomoc prijatelja\n5. Prekini igru\n')
    print('Odaberi...')
    while True:
        try:
            opcija = int(input())
            if opcija not in range(1, 6):
                raise ValueError
            break
        except ValueError:
            print('Opcija koju ste izabrali ne postoji\nPokusajte ponovo:')

    if opcija == 1:
        matrica = formiraj_matricu()
        print('Uspesno ste restartovali igru!')
        beskorisno = input()
    elif opcija == 2:
        talon(matrica, csr)
        beskorisno = input()
    elif opcija == 3:
        x = odigraj_potez(matrica, x)
    elif opcija == 4:
        #TODO Potez prijatelja
        'Ne postoji izabrana opcija'
    else:
        exit(0)