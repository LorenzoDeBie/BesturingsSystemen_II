# 4 Disks and filesystems

## Inleiding

Een filesysteem is een indexeringsmechanisme om bestanden te kunnen opslaan in een hiërarchische mappenstructuur.

In Windows voor elk device een stationsletter en individuele mappenstructuur.

In Linux is er 1 enkele logische mappenstructuur waar submappen omleiden naar het indexeringsmechanisme van een specifiek devide of geheugenstructuur.

Tijdens het parsen van het pad van een bestand moet het OS weten op welk device het zich bevind.  
Daarom is er een conversietabel nodig.  
Die het verband legt tussen het device (_source_) en het daarbijhorende indexeringsmechanisme (_fstype_)  
en anderzijds de submap in het filesysteem (_target_ of _mountpoint_).
Tijdens het mounten kunnen opties meegegeven worden.

Adhv de opdracht: `mount` of `findmnt` kan men deze tabel tonen. Deze is onder de vorm van:

```bash
device on /target type fstype (flags)
e.g. /dev/sda1 on /boot type ext4 (rw,relatime,data=ordered)
```

/proc, /dev/, /sys en de meeste anderen vereisen een specifiek indexeringsmechanisme zoals proc, devtmpfs en sysfs.  
De mappen met fstype tmpfs verwijzen naar fracties van het geheugen die RAM ter beschikking stellen.

> Mounten gebeurt dynamisch tegenwoordig

> In oudere versies werd het statisch geconfigureerd met behulp van een configuratietabel `/etc/fstab`  
> Iedere lijn bestaat uit 6 velden: source, target, fstype, opties, een optie om te bepalen of er een dump
> moet worden gemaakt bij een systeemcrash en een kolom die de mountvolgorde bepaald

**Note:** fstype _swap_ is het inverse van _tmpfs_ het bepaald welke sec. opslagdevices virtueel geheugen ter beschikking moeten stellen.

## 4.2.3 Mounting a Filesystem

`mount [opties] source target`  
Indien source of target weggelaten wordt dan zal mount de jusite info proberen achterhalen in `/etc/fstab`.  
Er mag een synoniem gebruik worden.  
Indien de target map niet leeg zou zijn blijft de inhoud ontoegankelijk zolang de mount er is.

Bij `unmount` volstaat 1 van beide argumenten.  
`-t` fstype optie (kan meestal achterhaald worden door het OS)  
`-o` mountopties  
Een aantal voorbeelden van deze opties zijn: `rw, suid, exec, auto, nouser`.  
Sommige van deze opties hebben enkel zin in het `/etc/fstab` bestand

Gebruik de optie `remount` om aanpassingen te maken aan de opties van een reeds gemounte target.

`mount` laat toe om te mounter via het LABEL of UUID van een device.

| CL option | Uitleg            |
| :-------: | :---------------- |
|    -t     | fstype            |
|    -a     | all               |
|    -f     | fake              |
|    -v     | verbose           |
|    -L     | label             |
|    -U     | UUID              |
|    -n     | zonder aanpassing |
|    -r     | read              |
|    -w     | write             |

`-n` optie bestaat omdat er in de eerste bootfases van linux nog geen aanpassingen kunnen gebeuren aan /etc/mtab.

## 4.2.9 Alternatives to /etc/fstab

Gebaseerd op _udev_.  
De **udisk2** daemon zal van zodra udev verneemt dat er bijkomende diskpartities zijn deze ergens mounten.  
Optie **UDISKS_IGNORE** zorgt ervoor dat er niet automatisch gemount wordt.  
Optie **UDISKS_FILESYSTEM_SHARED** zorgt ervoor dat de targetmap altijd /media/label wordt.

### Alternatief 2

`systemd mount units` en gebruik de `What=` en `Where=` properties voor configuratie.

## Voorbeeld over filesystem

Linux is een multiuser systeem.  
Dit wordt vooral gebruikt om daemons in hun eigen gebruikerscontext te laten draaien.  
Dit is belangrijk uit beveiligingsstandpunt waardoor fouten of hacks enigzins beperkt kunnen blijven.

Om de kans op zo inbruiken nog meer te beperken worden veel daemons uitgevoerd in een _chroot jail_.
Dit is een submap waaruit de daemon niet kan ontsnappen. De jail map is de root van de view van de daemon.  
In deze jail worden dan zo weinig mogelijk mappen geplaatst. Enkel het noodzakelijke.  
Er kunnen geen sybolische links gebruikt worden anders zouden deze gevolgd kunnen worden uit de jail.  
Daarom wordt er gebruik gemaakt van harde links.

Harde links leggen naar de programmas is niet altijd genoeg. Sommige bestanden hebben zelf nog een aantal bestanden nodig hiervoor gebruik je `ldd` om deze te vinden en dan kopieer je ze naar de jail.

> Note: harde links tussen mappen gaat niet.

De juiste functionaliteit kan ook bekomen worden aan de hand van `mount -B source target`.  
De optie `-B of -R` staat voor bind en respectievelijk rbind