# 3a Device Files

## 3.1 Device Files

Elk device heeft 2 representaties in de filesysteem hierarchie.

- Klassieke representatie als device file/node in `/dev`:  
  bedoeld om min of meer uniforme toegang te bieden aan toepassingsprogramma's
- Diverse paden in het sysfs virtual file system in `/sys`:  
  bedoeld om applicaties informatie te bezorgen vanuit usermode

`/dev` wordt tijdens boot dynamisch geconstrueerd.  
/dev zit vol met symbolische links, zodat je een device kan aanspreken met verschillende synoniemen.

```shell session
$ ls -l $(find /dev -type c -o -type b -o -type p -o -type s)
crw-------  1 root    root     10, 235 May 26 10:42 /dev/autofs
crw-------  1 root    root    252,   0 May 26 10:42 /dev/bsg/0:0:0:0
crw-------  1 root    root    252,   1 May 26 10:42 /dev/bsg/2:0:0:0
crw-------  1 root    root     10, 234 May 26 10:42 /dev/btrfs-control
...
```

In het resultaat van bovenstaande opdracht kan je in kolom 5 en 6 het major en minor number zien. Iedere combinatie van deze nummers is uniek. Ze duiden respectievelijk de driver en deviceinstantie aan.

> Je kan een device aanspreken met standaard IO systeemaanroepen

Het eerste lettertje van het resultaat duid het device type aan. Er zijn hier 4 mogelijkheden: **b**lock, **c**haracter, **p**ipe en **s**ocket.  
Pipe en socket devices hebben een eindige levensduur.

Er zijn veel meer character devices dan block devices. Hardware kan zowel via block als character aangesproken worden maar block is veel moeilijker te programmeren.  
Block devices bufferen, dit kan voor betere efficiëntie zorgen in bepaalde toepassingen.

---

`blockdev` geeft een overzicht van alle aanwezige block devices en de belangrijkste eigenschappen.

Om een deel van een bestand/backup/... te overlopen zonder het geheel te moeten kopieren naar de schijf kan je een loop device gebruiken.  
Gebruik `losetup` om een device aan te maken en mount het op de host.  
Verwijder de mount, de map en het loopdevice (`losetup -d ...`).

---

Character devices zijn niet-gebufferde IO devices.  
Deze worden gebruikt bij bv terminals (tty en pts).  
Bij stdout,... wordt er aan de hand van symbolische links duidelijk gemaakt welk device gebrukt wordt.

---

Je kan om het even welk in- of uitvoer kanaal omleiden adhv de `exec #< | #>` opdracht.  
Er is voor ieder proces een map met filedescriptoren `/proc/PID/fd`

| Operator        | Omschrijving                                         |
| :-------------- | :--------------------------------------------------- |
| exec #< ...     | lezen van een bestand of pipe device                 |
| exec #<<< ...   | lezen van een string variabele                       |
| exec #< <() ... | uit process substitution                             |
| exec #> ...     | schrijven van een bestand of pipe device             |
| exec #>> ...    | appenden van een bestand of pipe device              |
| exec #<> ...    | bestand in read/write modus (NOT DONE volgens Joris) |
| exec #<&-       | fd sluiten                                           |

> Hier kan ipv een nummer ook een {var} gebruikt worden als naam voor de fd.  
> Deze wordt dan gebruikt door <&\${var}

## 3.4.8 Creating Device Files

Vroeger toen device nodes statisch op de schijf werden opgeslagen moest je via `mknod` de node terug aanmaken indien je die verwijdert had.  
Dit commando kan nu ook nog gebruikt worden om named pipes te maken `mknod fifo p`.  
Dit kan ook met het commando `mkfifo fifo`.

## 11.7 Command Substitution

Process substitution zal de uitvoer van de opdracht of pipeline omleiden naar een pipe die door een andere opdracht kan benaderd worden als een bestand.
