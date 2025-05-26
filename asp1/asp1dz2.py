nacin=input("Unesite nacin ucitavanja:")
if nacin=="standardni ulaz":
    m=int(input("Unesite broj grupa pojmova:"))
    n=int(input("Unesite broj pojmova u jednoj grupi:"))
    lista = []
    pojmovi=[]
    svipojmovi=[]

    for i in range (0,m):
        elem = input("Unesite grupu pojmova: ").split(",")
        lista.append(elem)

    while (1):
        linija = input().strip()
        if linija != "":
            if "-" in linija:
                pojmovi=linija.split("-")
                pojmovi.insert(1,"-")
                svipojmovi.append(pojmovi)
            elif "+" in linija:
                pojmovi=linija.split("+")
                pojmovi.insert(1, "+")
                svipojmovi.append(pojmovi)
            else:
                raise ValueError
        else:
            break

    print(m, n, lista, svipojmovi, sep='\n')

if nacin=="tekstualni fajl":
    unos = input().strip()
    mn,lista, pojmovi, svipojmovi = [], [], [], []
    with open(unos,"r") as unos:
        ceoFajl = unos.readlines()

        for i in range (len(ceoFajl)):
            ceoFajl[i] = ceoFajl[i].strip()
            if ceoFajl[i].isdigit():
                mn.append(int(ceoFajl[i]))
            else:
                if "," in ceoFajl[i]:
                    elem = ceoFajl[i].split(",")
                    lista.append(elem)
                else:
                    linija = ceoFajl[i].strip()
                    if "-" in linija:
                        pojmovi = linija.split("-")
                        pojmovi.insert(1, "-")
                        svipojmovi.append(pojmovi)
                    elif "+" in linija:
                        pojmovi = linija.split("+")
                        pojmovi.insert(1, "+")
                        svipojmovi.append(pojmovi)
                    else:
                        print("Los unos")

    print(mn[0], mn[1], lista, svipojmovi, sep='\n')