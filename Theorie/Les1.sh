ls #list all files
pwd # print working directory
#ctrl + r = reverse search naar commando's met dit woord
factor #deel van unix of linux
shuf --input-range=0-9 --head-count=20 --repeat #random permutatie van getallen
shuf -i 0-9 -n 20 -r #korte versie van hierboven
shuf -ri 0-9 -n20 #opties zonder argumenten aan mekaar enzo, spaties is proberen
shuf #zonder opties of parameters --> wacht op input en dan door elkaar
shuf < FILE # geeft de inhoud van file mee aan programma
shuf -e een twee drie vier #argumenten meegegeven --> optie -e gebruiken om arg te shuffelen
shuf -e {0..9}
shuf -e * # * = naam alle bestanden in deze map
shuf -ze {a..z}
shuf -zi 0-9 #optie -z --> geen \n maar \0 als einde
shuf -n 14 -rze {{a..z},{A..Z},{0..9}," "} # good password
shuf -n 30 -re -- - + #-- zorgt ervoor dat er hierna geen opties meer gelezen worden
ps # geef lijst van lopende processen
X=een #variabele
echo $X #variabele gebruiken
declare X=een #variabele (zelfde als X=een)
declare -p X #geeft waarde van X
declare -p ${!STRING*} #geef mij alle vars die met STRING beginnen
#LET OP als string er niets gevonden wordt met STRING --> lege string meegegeven aan declare --> ik krijg alles!
