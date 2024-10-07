.386

.MODEL flat, C
.STACK 4096

.DATA
	clear_color DWORD 0000000h
    pixel_color DWORD 0FF00FFh  ; Example color (Red: 0xFF0000)
    pixel_x     DWORD 2         ; X coordinate
    pixel_y     DWORD 0         ; Y coordinate
    distance      DWORD 100h

.CODE

	update PROC
		mov eax, dword ptr[distance]
        add dword ptr[pixel_x], eax    ; Changing Distance value changes the pattern on screen I do not know why

		ret
	update ENDP


	render PROC

        mov eax, dword ptr[pixel_y]
        mov ebx, [esp + 12]             ; Store the pitch
        mul ebx                         ; eax = pixel_y * bitmap.pitch
        
        add eax, dword ptr[pixel_x]     ; Add the pixel_x offset

        mov ebx, [esp + 20]             ; Store the window bitmap

        cmp [esp + 16], eax             ; Make sure eax is not greater than the size of the bitmap
        jg continue

            ; Clear the screen
            push edi
            mov edi, ebx
            mov eax, 0
            mov ecx, [esp + 20]
            shr ecx, 2
            rep stosd
            pop edi

            ; Reset values
            mov pixel_x, 0
            mov eax, 0
            inc distance ; Cycle the next pattern.
            ret

        continue:
        add ebx, eax ; add the address offset

        mov eax, pixel_color        ; store the pixel color
        mov dword ptr[ebx], eax     ; Assign the address the pixel color

        ret
    render ENDP



END