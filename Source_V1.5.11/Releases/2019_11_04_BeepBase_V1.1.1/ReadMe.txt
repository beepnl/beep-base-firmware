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