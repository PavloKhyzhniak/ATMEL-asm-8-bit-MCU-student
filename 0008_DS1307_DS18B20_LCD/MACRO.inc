

.MACRO input ;��� ������ ��������
	.IF @1 < 0x40
		in @0, @1
	.ELSE
		lds @0, @1
	.ENDIF
.ENDM

.MACRO output ;��� ������ � �������
	.IF @0 < 0x40
		out @0, @1
	.ELSE
		sts @0, @1
	.ENDIF
.ENDM