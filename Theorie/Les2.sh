declare -p # toon alle vars
declare -- PAD="dit is een string" #maak var
echo $PAD #var gebruiken
declare -p x #toon inhoud van x
declare -x x #zorgt ervoor dat kind shell ook x kent
export x # zelfde als hierboven
declare +x x #x niet meer in environment

#configuratieparameters van de shell
set -o #toont parameters
#verbose en xtrace zijn noodzakelijk voor shell scripts
set +o #toont parameters maar in de vorm met opdrachten
set -o verbose #zet verbose aan
set -v #korte versie van hierboven
set +o verbose #zet verbose uit
set +v #korte versie van hierboven
bash -v -x script #roept script op met verbose en xtrace
set -o noexec #scripts worden niet uitgevoerd --> enkel syntax checken
set -o noclobber # geen bestaande bestanden overschrijven, enkel nieuwe maken
set -o nounset #no unset: geen empty response bij onbestaande var --> geeft fout
set -o errexit #sluit shell bij fout: kan handig zijn bij bv cd faalt en ik delete al mijn temp files (lees hele folder) --> shell afbreken ipv alles te deleten
echo $? #toont of laatste command fout gereturned heeft (ik denk return waarde van laatste command)
#niet posix opties
shopt #toont niet posix opties
shopt -p #toont niet posix opties via commands
shopt -s nocaseglob #case unsensitive zoeken naar patronen
shopt -s nocasematch #case unsensitive strings vergelijken
#bestanden die beginnen met '.' worden beetje weggestoken
shopt -s dotglob #toont ook alle files met . in ls
#.bashrc en .bash_profile voor opdrachten die 1 keer uitgevoerd moeten worden bij hoofdshell
ls -d @(*csv|*k) #toon alle files die eindigen op csv en k
ls -d !(@(*csv|*k)) #toon alle files die niet eindigen op csv en k
#bovenstaande commandos werken niet indien in file en bash voert uit want extglob staat af
shopt -s extglob #zet extglob aan: INDIEN EXTGLOB GEBRUIKT IN SHELL SCRIPT --> DIT BOVENAAN SCRIPT UITVOEREN
type -a printf #toont alle types en locaties van printf (builtin is altijd sneller dan externe)
\grep w lees #\opdracht omzeilt alias EXAMEN, werkt ook in shell scripts
unalias commando #doet alias perm weg
declare -p BASH_CMDS #toont alle commands die bash al eens moeten zoeken heeft en nu gehashed heeft
which -a printf #toont locaties van externe versies
#waarom soms externe gebruiken? externe kunnen nieuwere versies gebruiken --> meer opties ofzo: bv kill -L vs externe versie
enable -n opdracht #disable interne versie van opdracht
enable kill #enable interne versie van kill
#waar commandos zoeken? --> in var PATH
#windows path bevat '.' (huidige map) wel --> unix kan scripts uit deze map niet uitvoeren zonder './' ervoor te zetten
PATH=".:$PATH" #groot veiligheidsrisico want nu zal eerst script met naam uitgevoerd worden uit deze map (kan door malafide gebruiker gemaakt zijn)
echo "ditiseenstringmet*(" #hier worden alle characters ge-escaped (\ voor ieder char)
echo ditiseenstringmet\*\( #output zelfde als hierboven
#binnen dubbele quotes kan $variabele nog gebruikt worden, enkele quotes laten dit niet toe
#nog strengere manier van quotes:
echo $'donald \t \n \r daisy' #\t kan hierin wel gebruikt worden
#\r zet cursor terug naar begin van de lijn
#\x0B hex codes van ascii gebruiken (hier vertical tab)
#\cK eerste 26 chars van ascii gebruiken (A=1 B=2 enz)
echo $'donald \xce\xb1 daisy' #kleine alpha --> unicode gebruiken zoek bytes op en geef ze in met \x
shuf -n 30 -zre $'\xe2\x99\xa5'
x=0; while ((1)) ; do sleep 0.25; echo -n $'\r' ${ARROW[$((++x%8))]}; done
tput civis; #maak cursor invisible
tput cvvis; #maak cursos very visible
x=0; while ((1)) ; do sleep 0.25; echo -n $'\r' ${CLOCK[$((++x%24))]} $' '; done #examen

#filterprogramma = lezen op kanaal 0 (STDIN) en schrijven naar kanaal 1 (STDOUT)
#< leest invoer uit file
#<<< leest invoer uit variabele







