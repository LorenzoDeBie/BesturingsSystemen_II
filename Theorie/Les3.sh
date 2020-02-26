tr 'a-z' 'A-Z' #alle chars uit par1 omzetten naar chars uit par2 (kleine letters naar grote)
tr 'a-z' 'A-Z' 0 < file #0 omleiden naar file
wc w* 1> t1 2>&1 #fd 1 omleiden naar file t1 en fd 2 omleiden naar &1 (= waar 1 naar wijst), omleiden kan op iedere plaats in commando
1> t1 #beste manier om bestand leeg te maken: uitvoer omleiden naar bestand maar niets uitvoeren -_> bestand leeg
{ wc w* ;  ls ; } > t1 2> t2 # meerdere commandos uitvoeren, stdout naar t1 en stderr (fouten) naar t2
f () { wc w* ; ls ; } # functie definitie
f () { wc w* ; ls ; } > t1 2>&1 #functies kunnen ook omgeleid worden
shuf -n 10 -ri 0-9 | tee t1 t2 | sort | uniq -d # tee leest van stdin en stuurt naar alle files meegegeven en naar stdout
#uniq merged duplicate lines naar 1 line, met optie -d toont het gewoon alle duplicaten
#in de man page /pattern om te zoeken, q om te verlaten, h voor help
man -k pattern #zoeken in alle man pages naar pattern en toont overzicht van deze pages
man -f kill # toont alle man pages van kill
man -a kill # loopt door alle man pages van kill
info date #betere versie van man
shuf --help # nog een manier voor help
#Wat zijn interne opdrachten? --> zie type
help command # voor interne commandos
# optie -d --> korte beschrijven
# optie -s --> geef syntax
help -s if #help werkt ook voor structuren

#linux file systeem
#zonder slash = relatief
tree -d # tree toont alles in deze map, optie -d toont enkel directories
tree -dfi -L 2 /usr/share | tail -n +2 | head -n -2
# tail -n +2 = alles vanaf tweede lijn; tail = ik wil deel van laatste stuk
# head -n -2 = alles behalve laatste twee lijnen; head = ik wil deel van eerste stuk
ls -ld $(tree -dfi -L 2 /usr/share | tail -n +2 | head -n -2)
tree -d /proc | less #opent tree command in readonly vi --> scrollen en zoeken enzo mogelijk
echo 0 > /proc/sys/net/ipv4/ip_forward #kernel parameter wijzigen via de file (werkt niet bij alle kernel params)

yes y | rm -vi * 2> /dev/null

yes $'y\nn' | rm -vi * 2> /dev/null

shuf -r -e y n # random y en n 
