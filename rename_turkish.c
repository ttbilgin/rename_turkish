// rename_turkish.c
#include <windows.h>
#include <stdio.h>
#include <wchar.h>

void replace_char(wchar_t *str, wchar_t find, wchar_t replace) {
    while (*str) {
        if (*str == find) *str = replace;
        str++;
    }
}

void remove_combining(wchar_t *str) {
    wchar_t *src = str;
    wchar_t *dst = str;
    while (*src) {
        // Combining Diacritical Marks: U+0300 - U+036F
        if (*src < 0x0300 || *src > 0x036F) {
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = L'\0';
}

int wmain(int argc, wchar_t *argv[]) {
    if (argc < 2) {
        wprintf(L"Kullanim: rename_turkish.exe <dosya>\n");
        return 1;
    }

    wchar_t *filepath = argv[1];
    wchar_t filename[MAX_PATH];
    wchar_t dir[MAX_PATH];
    wchar_t newpath[MAX_PATH];

    // Dizin ve dosya adini ayir
    wchar_t *lastSlash = wcsrchr(filepath, L'\\');
    if (lastSlash) {
        wcsncpy(dir, filepath, lastSlash - filepath + 1);
        dir[lastSlash - filepath + 1] = L'\0';
        wcscpy(filename, lastSlash + 1);
    } else {
        dir[0] = L'\0';
        wcscpy(filename, filepath);
    }

    // Composed Turkce karakterleri degistir (NFC)
    replace_char(filename, 0x00E7, L'c');  // ç
    replace_char(filename, 0x00C7, L'C');  // Ç
    replace_char(filename, 0x011F, L'g');  // ğ
    replace_char(filename, 0x011E, L'G');  // Ğ
    replace_char(filename, 0x0131, L'i');  // ı
    replace_char(filename, 0x0130, L'I');  // İ
    replace_char(filename, 0x00F6, L'o');  // ö
    replace_char(filename, 0x00D6, L'O');  // Ö
    replace_char(filename, 0x015F, L's');  // ş
    replace_char(filename, 0x015E, L'S');  // Ş
    replace_char(filename, 0x00FC, L'u');  // ü
    replace_char(filename, 0x00DC, L'U');  // Ü

    // Combining karakterleri sil (NFD icin)
    remove_combining(filename);

    // Bosluk -> _
    replace_char(filename, L' ', L'_');

    // Yeni yolu olustur
    wcscpy(newpath, dir);
    wcscat(newpath, filename);

    // Yeniden adlandir
    if (_wrename(filepath, newpath) == 0) {
        wprintf(L"Basarili: %s\n", filename);
    } else {
        wprintf(L"Hata!\n");
        return 1;
    }

    return 0;
}