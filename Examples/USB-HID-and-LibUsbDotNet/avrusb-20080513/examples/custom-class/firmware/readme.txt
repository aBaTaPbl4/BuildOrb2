20 июля 2008
		Что-то никак не работает USB-устройство. Компьютер пишет, что "One of the USB devices attached to this computer has malfunctioned, and Windows does not recognize it. For assistance in solving this problem, click this message.". Решил начать разбираться в коде, для начала выкинуть из него все ненужное (например, модуль usbdrv\oddebug.c).

21 июля 2008
		Оказалось, дело не в коде - я допустил 2 аппаратные ошибки. После устранения их все заработало. Ошибки такие:
		1. Не работал кварцевый генератор из-за неправильно записанных перемычек.
		2. Перепутал сигналы D+ и D-.
		