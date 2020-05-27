# 3b sysfs & udev

## 3.1 Device Files

Elk device wordt gerepresenteerd in `/dev` en in `/sys`. Hoe deze gemapt worden hangt af van het soort filesysteem.  
Bij _/dev_ is dit devtmfs en bij _/sys_ is dat sysfs.  
De _/sys_ representatie heeft als doel om eigenschappen, attributen, van het device ter beschikking te stellen vanuit de gebruikersmodus.

Elk device heeft 1 map in _/sys_ dit is het **syspath**. Dit **syspath** wordt in kernel modus gecreëerd als het device wel degelijk aanwezig is.

Ook wordt er nog extra doorverwezen aan de hand van symbolische links naar de werkelijke syspaths.

De attributen van een device worden elk gerepresenteerd door een bestand in de hiërarchie onder het syspath.

## 3.2 The sysfs Device Path

Gebruik `udevadm info -q path /dev/...` om het syspath van een device te vinden. (Let op: er moet hier nog /sys/ voorstaan).  
Het omgekeerde kan door `udevadm info -q name /sys/...` te gebruiken.

Indien een syspath in de hiërarchie zit van een ander syspath dan betekend dit dat overeenkomstig device afhankelijk is voor zijn werking van het parent device dat overeenkomt met het andere syspath.

Attribuut bestanden zitten ofwel in het syspath zelf of 1 niveau eronder.  
Bijna alle bestanden bestaan maar uit 1 lijn.

De sysfs devicemappen worden dynamisch door de kerner gecreëerd.

> Een aantal tools om attributen van een device te verzamelen zijn: `lspci -v` , `lsscsi -v` en `lsusb -v`.  
> Deze zijn afhankelijk van het soort device.

`lsblk` geeft een overzicht van alle block devices en enkele eigenschappen (optie -OP om alle eigenschappen weer te geven)

Er is ook een generische tool `udevadm info`. Deze intereageert met een device manager.  
Als je dit commando uitvoert op 1 device krijg je volgende uitvoer:

```shell session
$ udevadm info /dev/sda1
P: /devices/pci0000:00/0000:00:0d.0/ata1/host0/target0:0:0/0:0:0:0/block/sda/sda1
N: sda1
S: disk/by-id/ata-VBOX_HARDDISK_VBa2cdf550-fbbc2b38-part1
S: disk/by-uuid/db43365b-70a0-4b77-b573-9d83fdf3b186
E: DEVLINKS=/dev/disk/by-id/ata-VBOX_HARDDISK_VBa2cdf550-fbbc2b38-part1 /dev/disk/by-uuid/db43365b-70a0-4b77-b573-9d83fdf3b186
E: DEVNAME=/dev/sda1
E: DEVPATH=/devices/pci0000:00/0000:00:0d.0/ata1/host0/target0:0:0/0:0:0:0/block/sda/sda1
...
E: USEC_INITIALIZED=3724
```

Hier zijn er 4 soorten lijnen:

| Lijn | Omschrijving                                                      |
| :--: | :---------------------------------------------------------------- |
|  P   | het syspath                                                       |
|  N   | de orginele /dev/node naam                                        |
|  S   | alternatieve /dev/node namen                                      |
|  E   | aanvullende properties die in de udev databank worden bijgehouden |

Als je slechts 1 van deze 4 soorten lijnen wilt dan geef je een extra optie `-q path/nam/symlink/property` mee.

Niet alle devices hebben een /dev/node representatie, deze kunnen dus niet via eenvoudige file IO aangesproken worden.

## 3.5 udev

Problemen met statische /dev/nodes:

- fouten in kernel modus dikwijls fataal
- niet geschikt voor alle devices
- mapping node/device na elke boot mogelijk anders
- geen hotplug mechanisme
- kernel heeft drivers nodig voor elk potentieel device
- device nodes worden gecreëerd voor alle ondersteunde devices -> groot aantal overbodige nodes
- beperkt aantal major numbers

De huidige oplossing werkt met `devtmpfs` dat wordt gemount tijdens de bootfase in `/dev`.  
Hiervoor is de udev daemon belangrijk, deze draait continu in de gebruikersmodus.

> Gebruik `udevadm monitor -k` om kernel uevents te monitoren.  
> Met de optie -p zie je ook de udev geïnterpreteerde inhoud van het bericht

Eens de udev daemon getriggerd wordt kunnen er allerlei acties ondernomen worden.

- De udev databank, die bestaat uit naam waarde paren, kan worden bijgewerkt.
- De machtigingen van de /dev/node kunnen gewijzigd worden
- Er kunnen bijkomende synoniemen gecreërd worden
- Er kunnen device drivers geladen worden
- Er kunnen andere user mode apps op de hoogte gebracht worden. (`udevadm monitor -u -p`)

Voordelen huidige oplossing:

- fouten niet fataal
- geschikt voor alle devices
- dynamische naamgeving die consistent blijft
- enkel drivers en /dev/nodes voor aanwezige devices
- applicaties op de hoogte van major numbers

### 3.5.2 udevd Configuration

Configuratie van de de udev daemon gebeurd aan de hand van de bestanden in de `/etc/udev/rules.d/` map.  
Elke lijn in zo een bestand met suffix _.rules_ correspondeert met 1 rule.  
De basis configuratie staat in de `/lib/udev/rules.d/` directory maar aanpassingen moeten gebeuren in de `/etc` versie, dan wordt de `/lib` versie genegeerd.

Bij elk _uevent_ laadt de udev daemon eerst alle sysfs attributen en worden alle udev rules gecheckt.  
Elke rule koppelt 1 of meer conditionals aan 1 of meer directives. Directives van een rule worden uitgevoerd indien alle conditionals true zijn.

- De **KERNEL** conditional checkt de door de kernel ingestelde /dev/node naam. De **KERNEL** key kan niet gebruikt worden als directive.
- De **ATTRS** conditionals bepalen of bepaalde attributen de juiste waarden hebben.
- De directive **SYMLINK** zorgt dat er bijkomende synoniemen in /dev gecreëerd worden.

Directives en conditionals bestaan uit sleutel/waarde koppels.  
`==` kan gebruikt worden bij conditionals om te vergelijken.

> Om een willekeurige string aan te duiden gebruik je `?*`  
> Indien de sleutelnaam wordt aangevuld met een `S` dan mag de conditional voldaan worden door een parent device.

Directives gebruiken `=` of `+=` om te assignen.

Een overzicht van de belangrijkste sleutels:

| Sleutel              | Omschrijving                                            |
| :------------------- | :------------------------------------------------------ |
| KERNEL               | de default door kernel ingestelde /dev/node naam        |
| SYMLINK              | lijst van in te stellen synoniemen in /dev              |
| ACTION               | add/remove                                              |
| SUBSYSTEM            | devicetype                                              |
| MODE, GROUP en OWNER | permissions                                             |
| ATTR{attribuut}      | willekeurige sysfs attributen                           |
| ENV{proptery}        | willekeurige properties                                 |
| IMPORT               | diverse manieren om key-value pairs te genereren        |
| NAME                 | instellen naam netwerkinterface                         |
| PROGRAM en KEY       | programma bepaalt inhoud KEY (resultaat komt in RESULT) |
| DRIVER               |                                                         |
| RUN                  | uitvoeren willekeurig programma                         |

> Attributen zijn direct beschikbaar (nog voor udev begint aan de verwerking van de regels)  
> Properties daarentegen worden in cascade aangemaakt en gewijzigd. Sommige zullen dus nog niet beschikbaar zijn in het begin.

`IMPORT{builtin}` verwijst naar een interne functie van de udev daemon  
`IMPORT{program}` verwijst naar externe programma's van de udev daemon

---

Indien de kernel een device detecteert dat ondersteuning vereist wordt de noodzakelijke ondersteuning gecodeerd als value in het _modalias_ attribuut.

Telkens er uitvoerbare code van een loadable module op je systeem geïnstalleerd wordt, komt deze terecht in de `/lib/modules/$(uname -r)` directory.  
Met `modinfo` kan je alias lijnen zien met de devices die de module kan ondersteunen. Maar ook het depends sleutelwoord dat kan verwijzen naar andere noodzakelijke modules.

`modules.alias` bevat een gecompileerde versie van de modinfo informatie van alle bestanden.

Om deze twee aspecten aan elkaar te koppelen wordt in _80-driver.rules_ de modalias property gebruikt die ervoor zal zorgen dat als er nog geen driver actief is het programma `kmod load $env{MODALIAS}` wordt gerunt

---

Voor alle devices komen de namen van loadable modules voor als submap van een _drivers_ map.

> Met de bestanden `bind` en `unbind` kan je de module vragen de driverondersteuning van een device te onderbreken of te activeren.

```shell session
$ echo -n 0000:00:0x.y > /sys/bus/pci/drivers/module/unbind
$ echo -n 0000:00:0x.y > /sys/bus/pci/drivers/module/bind
```

---

Om de aanpassingen van `/etc/udev/rules.d` door te voeren kan je de volgende commando's samen uitvoeren:

```shell session
$ udevadm control -R ; udevadm trigger
```

`udevadm test -a add|remove /sys/...` kan de veranderingen simuleren.
