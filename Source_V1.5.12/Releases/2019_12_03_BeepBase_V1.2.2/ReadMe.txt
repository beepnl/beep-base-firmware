2019_12_03 Release van Versie 1.2.2
	Fix voor het probleem met nieuwere smartphones die geen pin-code pop-up krijgen, omdat de encryptie mislukt.

2019_12_02 Release van Versie 1.2.1
	Recompile van release 1.2.0, maar dan zonder Pin code. Blijkt dat dit toch niet correct werkt op andere devices dan waar ik op heb getest.

2019_11_29 Release van Versie 1.2.0
	Flash logging toegevoegd voor meet data met als basis de Image Transfer voorbeeld: https://github.com/NordicPlayground/nrf52-ble-image-transfer-demo
		Voor de app ontwikkeling kan ook nog naar deze link worden gekeken: https://github.com/NordicPlayground/Android-Image-Transfer-Demo
		De snelheid is niet bepaald hoog met nRF Connect op een S5, maar dit komt grotedeels door de lage interval connectie en de MTU grote. Op mijn S5 wil het OS geen andere
		parameters instellen dan de instellingen bij het verbinden. De developer(https://devzone.nordicsemi.com/f/nordic-q-a/47503/bluetooth-5-0-file-transfer) van de OTS service zegt 
		snelheden te hebben behaald van 1200kbps, dus misschien dat er vanuit de App meer mogelijk is dan dat ik vanuit de peripheral kan behalen qua connectie interval.
		Ik heb wel de MTU standaard op 240 btes gezet ipv 23, omdat het onderhandelen met de client (S5 met nRF Connect) niks opleverde.
		
	BLE commands toegevoegd voor de Bootcount, FLASH_READ, FLASH_ERASE, FLASH_SIZE
	
	TX karakteristiek toegevoegd aan de BEEP service.
	
	Uitlezen van het flash log via de Beep control point en TX data karakteristiek
	
	Pincode bescherming aangezet. Pincode kan worden gereset door de reedswitch voor 30 seconden te bekrachtigen. De BEEPBASE geeft een pieptoon als deze is gereset.
		Dit betekend ook dat bonding nu verplicht is. Als een van de twee devices een vorige bond is vergeten kan dit een aantal pogingen kosten voordat de pincode pop-up komt. Als
		Encryptie mislukt op de BEEPBASE wordt de conenctie verbroken. Dit houdt in dat het verbinden een aantal keer herhaald moet worden voordat of de bestaande bond eerst gewist
		moet worden.
		Het bekrachtigen van de reedswitch wist alle bonden op de BEEPBASE.
	

2019_11_11 Release van Versie 1.1.1
	Release 1.1.1 gemaakt met de buzzer geactiveerd in firmware en een extra default tune op index 2. Beep protocol commando: 0x9102
	Programeer script aangepast zodat deze niet langer verwacht dat de hex file in een map genaamd "release staat."
	Firmware release 1.1.0 voor het gemak toegevoegd, aangezien die bijna gelijk is.
	Het probleem van de ontbrekende DS18B20 temperatuur in de LoRaWAN berichten was heel simpel: De decoder verwachte meer bytes dan er nog in het bericht aanwezig waren en stopte met decoderen.
	De temperatuur zat dus wel in het LoRaWAN bericht, maar in alle haast zag ik het DS18B20 READ commando niet in het hexadecimale bericht.

2019_11_04 Release van versie 1.1.0
	Eerste release van firmware.
	Bij verdere releases zal in dit document veranderingen worden bijgehouden.
	Beep Base Handleiding ge-update naar aanleiding van wijzigen van HX711 read and Write conversion command, HX711 state R/W command toegevoegd, Buzzer custom en default tune commands toegevoegd.
	
	
2019_11_01
	Vrijgave Beep Base handleiding 1V0