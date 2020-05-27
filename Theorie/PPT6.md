# 6 How user space starts

## 6.1 Inleiding

Daemons = achtergrond processen die services aanbieden  
(uitgevoerd in usermode)

Sinds 2015 meeste systemen overgestapt van System V init naar systemd init.

## 6.2 System V Runlevels

Voor elke daemon is er een shell script met de naam van deze daemon in map `/etc/rc.d/init.d`

```shell session
$ ls /etc/rc.d/init.d
```

Het script moet 1 argument toelaten, start of stop (of ...).  
Dependencies moeten door het script opgehaald worden.  
Zo een script ziet er uit als volgt:

```bash
start() {
    [ -z "$HIDDENARGS" ] && exit 0
    echo -n $"Starting NAME: "
    daemon /urs/bin/NAME $HIDDENARGS
    touch /var/lock/subsys/NAME
    echo
}
stop() {
    echo -n $"Shutting down NAME: "
    /usr/bin/NAME -K
    killproc NAME
    rm -f /var/lock/subsys/NAME
    echo
}

[ -f /usr/bin/NAME] || exit 0

case "$1" in
    start)
        start ;;
    stop)
        stop ;;
    restart|reload)
        stop
        start ;;
    status)
        status NAME ;;
    *)
        echo "Usage..."
        exit 1 ;;
esac
exit 0
```

---

Bij een reboot moeten sommige daemons automatisch opgestart worden.  
Zo worden er verschillende deelverzamelingen gedefinieerd, geïdentificeerd door een runlevel dat bij volgende reboot zal aangeven welke daemons worden opgestart.  
Met de `init` opdracht kan overgegaan worden naar een ander runlevel.  
Met `init 0` wordt het systeem uitgeschakeld.  
Met `init 6` wordt het systeem volledit opnieuw gestart.

De eerste 3 tekens in de filename zijn belangrijk

| Opties | Betekenis                               |
| :----: | :-------------------------------------- |
|   S    | actief op dit runlevel                  |
|   K    | niet actief op dit runlevel             |
|   XX   | getallen bepalen volgorde van opstarten |

## 6.4 systemd

### Voordelen systemd tov System V init

- niet alleen beheer van daemons maar ook van sys-status, networkresources, devices, mntpoints, ...
- geen shell scripting vereist: betere syntax
- afhankelijkheden beschrijven, onafhankelijk van volgorde waarin ze opstarten
- parallelle opstart van services -> snellere opstarttijden
- services starten getriggerd door events
- mogelijkheid voor automatisch herstarten van gestopte daemons
- eenvoudiger te beveiligen
- mogelijk om resources te beperken tot individuele units of groepen van units
- uniform beheer
- uniforme logging
- poging om verschillen tussen distributies weg te werken

### 6.4.1 Units and Unit Types

Diverse types componenten worden voorgesteld door **units**  
Units worden geconfigureerd door **unit bestanden**  
Met elke daemon komt een service unit overeen NAME.service

```shell session
$ systemctl list-units -a
    # Optie -a zorgt ervoor dat ook units geteld worden die niet actief zijn maar wel in geheugen staan.
$ systemctl list-units-files
    # Geeft alle unit config bestanden
$ systemctl list-unit-files -t service
    # Geeft de unit files van een specifiek type
```

#### Unit types

- **service:** daemons
- **target:** groeperen van andere units  
   `init 0` kan geschreven worden door `systemctl isolate poweroff.target`

| Runlevels | Target Units      |
| :-------: | :---------------- |
|     0     | poweroff.target   |
|     1     | rescue.target     |
|     3     | multi-user.target |
|     5     | graphical.target  |
|     6     | reboot.target     |

- **timer:** alternatief voor cron, start unit op als de timer afloopt  
  kan eenmalig of periodiek triggeren op absolute of relatieve tijdstippen
- **device:** device unit die gecreëerd wordt door udev rules in te stellen

> De naamgeving van een device unit wordt bepaald door het syspath van het device.  
> Zonder de eerste /, en waarbij alle andere /'s door -'s zijn vervangen,bijvoorbeeld:  
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;sys-devices-pci0000:00-0000:00:0d.0-ata1-host0-target0:0:0-0:0:0:0-block-sda-sda3.device  
> of  
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;sys-subsystem-net-devices-eth0.device

- **socket:** monitort netwerk of FIFO activiteit, vooral gebruikt tijdens opstarten daemon van network service  
  zorgt ervoor dat de daemon kan herstart worden zonder verlies van aanvragen
- **path:** triggeren de geassocieerde unit (map of bestand)
- **(auto)mount, swap:** laat (un)mounten toe via units (door in /etc/fstab vermelde regels of indien een van de paden aangesproken wordt)
- **snapshot:** gecreëerd via `systemctl status` maakt rollback naar momentopname van systemd toestand mogelijk
- **slice:** laat accounting toe en beperking van systeembronnen

### 6.4.4 systemd Operation

```shell session
$ systemctl is-active
$ systemctl is-enabled
$ systemctl is-failed
```

Daemons kunnen dynamisch gewijzigd worden, hiervoor is een reload subopdracht ter beschikking.

---

Configuratiebestanden van units kunnen in fragmenten wordne opgeslagen

### 6.4.2 systemd Dependencies

Bestanden zoals `/etc/systemd/system/....target.wants` identificeren alle services die aan de target geassocieerd worden.

**Wants, Requires, BindsTo:** vermeld gewenste andere units. Deze worden parallel met zichzelf gestart.  
Faalt dit dan faalt de unit zelf ook, stopt een andere dan stopt de huidige ook (bij **BindsTo**). (Laatste 2 niet bij **Wants**)

Om een nieuwe unit te kunnen starten bij een systeem boot moet deze dus voorkomen in een **Wants** property van een van de units die starten bij startup.  
Dit gaan aanpassen in de **Wants** van een andere unit is _not done_ op die manier.

Dus eigen config file aanvullen met **WantedBy** property met hierin de naam van de unit waardoor ze wensen opgestart te worden.

---

**Wants, Requires, BindsTo** vermelden vooral naar:

- slices
- targets
- (services maar deze verwijzen dan meestal naar targets)
- mounts en sockets

**WantedBy, RequiredBy** vermelden enkel naar:

- targets
- (services maar deze verwijzen dan meestal naar targets)

Het is hierbij niet genoeg om enkel de symbolic link te maken, de unit moet nog enabled worden.  
Dit gebeurd soms impliciet, anders `systemctl enable NAME.service`

Deze dependencies kunnen getoon worden aan de hand van `systemctl list-dependencies [--reverse]`.

### 6.4.8 System V Compatibility

`systemctl isolate` start alle units waarvan target **WEL** afhankelijk is en stopt alle andere.  
Dit is equivalent aan `init #`.

`systemctl get-default` is het _default runlevel_

### 6.4.5 Adding Units to systemd

Elke unit moet individueel geconfigureerd worden door een unit bestand met dezelfde naam als de unit.
_systemd_ gaat dit bestand gaan zoeken in 2 mappen

- `/etc/systemd/system` deze heeft voorrang
- `/usr/lib/systemd/system` deze mogen niet aangepast worden!!

> Om een unit te verwijderen `systemctl mask` uitvoeren of een leeg bestand voor deze unit plaatsen in `/etc/systemd/system`

#### Opbouw unit bestad

##### [Unit] sectie

Bevat **Wants** (en gelijkaardige).  
Wordt geïnterpreteerd bij opstarten van de service

Enkele directives (allemaal niet verplicht):

| Directive                       | Omschrijving                                                    |
| :------------------------------ | :-------------------------------------------------------------- |
| Description=                    | informatieve beschrijving                                       |
| Documentation=                  | specifieke manpagina en/of URL's (getoond met systemctl status) |
| Wants=, Requires=, BindsTo=,... | bepalen de overeenkomstige dependencies                         |
| Conflicts=                      | stopt/start unit indien vermelde unit gestart/gestopt wordt     |
| After=, Before=                 | bepaalt opstartvolgorde maar geen dependency                    |
| Condition...=                   | legt voorwaarden op om de unit te starten                       |

##### [Unittype] sectie

Bijvoorbeeld **[Service]**.  
Bevat informatie in verband met het uit te voeren programma.  
Kan ook vermelden dat er parallell mag gewerkt worden.  
Kan ook vermelden dat de service opnieuw moet gestart worden indien gestopt.

##### [Install] sectie

Bevat **WantedBy** (en gelijkaardige).  
Deze worden enkel geparsed bij enablen van de unit.

Deze sectie is niet verplicht. (Dan krijgt de unit de property `UnitFileState=static`)  
Enkel expliciet door andere unit gestart worden.

Enkele directives:

| Directive   | Omschrijving                                               |
| :---------- | :--------------------------------------------------------- |
| WantedBy=   | verwijst (meestal) naar target units                       |
| RequiredBy= | strikte variant WantedBy                                   |
| Alias=      | creërt synoniem                                            |
| Also=       | creëert groep units die als geheel enabled/disabled worden |
