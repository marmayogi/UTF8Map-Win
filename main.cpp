﻿//
// This Conversion program was written by Marmayogi, Astrologer and Palmist, Sri Mahakali Jyothida Nilayam, Coimbatore, India.
// This Converts TTF to a Type 2 CIDFont, with base font Type 42, which can be accessed through a postscript program.
// This is particulary useful for Indian Languages having glyphs in excess of 255 in the character set.
// 
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "mapunicode.h"

short u2ps(const ELang pLan, const EMyFont pMyFont, const uint32_t pUnicodeQuad[4], short &pCntUnicode, uint16_t pCID[3])	// Transform Unicode to Postscript character code (2 bytes CID) for Type 42 base font based on Language.
{
	//
	// This function transforms Unicode Points to Postcript character identifiers (2 bytes CID).
	// This function is called by strps().
	//
	// 1) pLan is an input parameter of type ELang supplying language of the Font.
	// 2) pMyFont is an input parameter of type EMyFont indicating name of the font.
	//    For example, Noto font, Vijaya font, Nirala font, etc.
	// 3) pUnicodeQuad is an input array parameter with cells 4. This supplies atmost a quad of consecutive unicode points.
	//    If first unicode point is not a ligature or conjunct, then second unicode point will not be used.
	//    For example, if the first unicode point of the pair is a vowel, then next point will not be processed.
	//    In Tamil Language, to process ஸ்ரீ, four unicode point are required.
	//    In Tamil Language, to process க்ஷ, three unicode point are required.
	// 4) pCntUnicode is an input/output reference parameter indicating the number of Unicodes points present in the parameter pUnicodeQuad.
	//    i)  As an input parameter, it indicates the number of unicode points present in the parameter pUnicodeQuad. This value ranges between 1 and 4.
	//    ii) As an output parameter, it passes out the number of Unicode points untouched in the parameter pUnicodeQuad. This value ranges between 0 and 3.
	// 5) pCID is an output array with cells 3. This passes out atmost three CIDs.
	//    Suppose first unicode point is a vowel or Latin character, then this array will pass out only one CID.
	//    For ligatures, upto 3 CIDs will be passed out.
	// 
	// This function returns number of CIDs passed out through parameter pCID. This varies between 1 and 4.
	//

	if (pLan < ELang::eEnglish || pLan > aLanguage[cMaxLanguage]) return 0;		// Language is out of bounds.
	short cntCID = 0;															// Number of CIDs passed out through parameter pCID. This varies between 1 and 4.
	switch (pLan) {
		case ELang::eTamil:					//  2. Tamil language
		{
			switch (pMyFont) {
				case EMyFont::eNoToSansTamil_Regular:				// Google's Tamil Font (Regular)
				case EMyFont::eNoToSansTamil_Bold:					// Google's Tamil Font (Bold)
				{
					const uint16_t lcOffset_BasicLatin = 259;											// Basic Latin Offset w.r.t. aNotoSansTamilMap.
					const uint16_t lcOffset_Latin_1_Supplement = 387;									// Latin-1 Supplement Offset w.r.t. aNotoSansTamilMap.
					const uint16_t lcOffset_Latin_Extended_A = 515;										// Latin Extended-A Offset w.r.t. aNotoSansTamilMap.
					const uint16_t lcOffset_General_Punctuation = 643;									// General Punctuation Offset w.r.t. aNotoSansTamilMap.
					const uint16_t lcOffset_Currency_Symbols = 755;										// Currency Symbols Offset w.r.t. aNotoSansTamilMap.
					const uint16_t lcOffset_Tamil_Ligature=128;											// Tamil_Ligature Characters Offset w.r.t. aNotoSansTamilMap.
					const uint16_t lcOffset_கி = lcOffset_Tamil_Ligature + 32;							// Offset w.r.t. aNotoSansTamilMap.
					const uint16_t lcCID_க = 18;														// CID for glyph க (U+0B95).
					const uint16_t lcCID_க் = 77;														// CID for glyph க்.
					const uint16_t lcCID_க்ஷ = 76;														// CID for glyph க்ஷ.
					const uint16_t lcCID_ஸ்ரீ = 163;														// CID for glyph ஸ்ரீ.
					const uint16_t lcCID_ா = 41;														// CID for glyph ா (U+0BBE)
					const uint16_t lcCID_ௗ = 54;														// CID for glyph ௗ (U+0BD7)
					const uint16_t lcCID_ெ = 46;														// CID for glyph ெ (U+0BC6)
					const uint16_t lcRangeOffset_1 = pUnicodeQuad[0] - aUnicode[(int)pLan].blockBeg;	// CID corresponding to unicode point pUnicodeQuad[0].
					const uint16_t lcRangeOffset_2 = pCntUnicode > 1 ? pUnicodeQuad[1] - aUnicode[(int)pLan].blockBeg : 0;	// CID corresponding to unicode point pUnicodeQuad[1].
					const uint16_t cid_1 = aNotoSansTamilMap[lcRangeOffset_1];							// CID corresponding to unicode point pUnicodeQuad[0].
					const uint16_t cid_2 = aNotoSansTamilMap[lcRangeOffset_2];							// CID corresponding to unicode point pUnicodeQuad[1].
					const bool isBasicLatin = pUnicodeQuad[0] >= aUnicode[(int)ELang::eEng].blockBeg && pUnicodeQuad[0] < aUnicode[(int)ELang::eEng].blockEnd/2;	// Basic Latin
					const bool isLatin_1_Supplement = pUnicodeQuad[0] >= 0x00A0 && pUnicodeQuad[0] <= 0x00FF;	// Latin-1 Supplement
					const bool isLatin_Extended_A = pUnicodeQuad[0] >= 0x0100 && pUnicodeQuad[0] <= 0x017F;		// Latin Extended-A
					const bool isGeneralPunctuation = pUnicodeQuad[0] >= 0x2000 && pUnicodeQuad[0] <= 0x206F;	// General Punctuation Block: U+2000 to U+206F
					const bool isCurrencySymbol = pUnicodeQuad[0] >= 0x20A0 && pUnicodeQuad[0] <= 0x20CF;		// Currency Symbols Block: U+20A0 to U+20CF
					const bool isக்ஷ = pCntUnicode > 2 && pUnicodeQuad[0] == 0x0B95 && pUnicodeQuad[1] == 0x0BCD && pUnicodeQuad[2] == 0x0BB7;	// க்ஷ Aksharam
					const bool isஸ்ரீ = pCntUnicode == 4 && pUnicodeQuad[0] == 0x0BB8 && pUnicodeQuad[1] == 0x0BCD && pUnicodeQuad[2] == 0x0BB0 && pUnicodeQuad[3] == 0x0BC0;	// ஸ்ரீ Aksharam
					const bool isconsonant = pUnicodeQuad[0] >= 0x0B95 && pUnicodeQuad[0] <= 0x0BB9;	// between க and ஹ
					const bool isVowelSign_1 = pCntUnicode > 1 && pUnicodeQuad[1] == 0x0BCD;								// Consonant ending with ்.
					const bool isVowelSign_2 = pCntUnicode > 1 && pUnicodeQuad[1] == 0x0BBE;								// Consonant ending with ா.
					const bool isVowelSign_3 = pCntUnicode > 1 && pUnicodeQuad[1] >= 0x0BBF && pUnicodeQuad[1] <= 0x0BC2;	// Consonant ending with ி,	ீ,	ு 	and ூ
					const bool isVowelSign_4 = pCntUnicode > 1 && pUnicodeQuad[1] >= 0x0BC6 && pUnicodeQuad[1] <= 0x0BC8;	// Consonant ending with ெ, 	ே and ை
					const bool isVowelSign_5 = pCntUnicode > 1 && pUnicodeQuad[1] >= 0x0BCA && pUnicodeQuad[1] <= 0x0BCB;	// Consonant ending with ொ,and ோ 
					const bool isVowelSign_6 = pCntUnicode > 1 && pUnicodeQuad[1] == 0x0BCC;								// Consonant ending with ௌ
					//for (short kk = 0; kk < pCntUnicode; kk++) { printf("\tuni[%d]=0X%04x", kk, pUnicodeQuad[kk]);}  printf("\n"); //getchar();
					if (isBasicLatin) {// Basic Latin Block is from U+0000 to U+00FF, but allowed range is from U+0000 to U+0080.
						const uint16_t offset = pUnicodeQuad[0] - aUnicode[(int)ELang::eEng].blockBeg;
						pCID[0] = aNotoSansTamilMap[lcOffset_BasicLatin + offset];				cntCID = 1;				pCntUnicode -= 1;
					}
					else if (isLatin_1_Supplement) {// Latin-1 Supplement Block is from U+0080 to U+00FF.
						const uint16_t offset = pUnicodeQuad[0] - 0x0080;
						pCID[0] = aNotoSansTamilMap[lcOffset_Latin_1_Supplement + offset];		cntCID = 1;				pCntUnicode -= 1;
					}
					else if (isLatin_Extended_A) {// Latin Extended-A Block is from U+0100 to U+017F.
						const uint16_t offset = pUnicodeQuad[0] - 0x0100;
						pCID[0] = aNotoSansTamilMap[lcOffset_Latin_Extended_A + offset];		cntCID = 1;				pCntUnicode -= 1;
					}
					else if (isGeneralPunctuation) {// General Punctuation Block: U+2000 to U+206F.
						const uint16_t offset = pUnicodeQuad[0] - 0x2000;
						pCID[0] = aNotoSansTamilMap[lcOffset_General_Punctuation + offset];		cntCID = 1;				pCntUnicode -= 1;
					}
					else if (isCurrencySymbol) {// Currency Symbols Block: U+20A0 to U+20CF.
						const uint16_t offset = pUnicodeQuad[0] - 0x20A0;
						pCID[0] = aNotoSansTamilMap[lcOffset_Currency_Symbols + offset];		cntCID = 1;				pCntUnicode -= 1;
					}
					else if (isக்ஷ) {// க்ஷ
						pCID[0] = lcCID_க்ஷ;		cntCID = 1;				pCntUnicode -= 3;
					}
					else if (isஸ்ரீ) {// ஸ்ரீ
						pCID[0] = lcCID_ஸ்ரீ;			cntCID = 1;				pCntUnicode -= 4;
					}
					else if (isconsonant && isVowelSign_1) {// Pulli - ்
						const short offsetFrom_க = cid_1 - lcCID_க;			// offset w.r.t letter க.
						pCID[0] = lcCID_க் + offsetFrom_க;					cntCID = 1;		pCntUnicode -= 2;
					}
					else if (isconsonant && isVowelSign_2) {// Kaal - ா
						pCID[0] = cid_1;		pCID[1] = cid_2;			cntCID = 2;		pCntUnicode -= 2;
					}
					else if (isconsonant && isVowelSign_3) {//	ி,	ீ,	ு 	and ூ
						const short idx = pUnicodeQuad[1] - 0x0BBF;
						const short offsetFrom_க = cid_1 - lcCID_க;			// offset w.r.t letter க.
						const short offset = offsetFrom_க * 4 + idx + lcOffset_கி;
						//printf("idx=%d offsetFrom=%d offset=%d\n", idx, offsetFrom_க, offset);
						if (aNotoSansTamilMap[offset]) {
							pCID[0] = aNotoSansTamilMap[offset];		cntCID = 1;
						}
						else {
							pCID[0] = cid_1;		pCID[1] = cid_2;		cntCID = 2;
						}
						pCntUnicode -= 2;
					}
					else if (isconsonant && isVowelSign_4) {// 	ெ,	ே and ை
						pCID[0] = lcCID_ெ + pUnicodeQuad[1] - 0x0BC6;		pCID[1] = cid_1;		cntCID = 2;		pCntUnicode -= 2;
					}
					else if (isconsonant && isVowelSign_5) {// 	ொ and ோ
						pCID[0] = lcCID_ெ + pUnicodeQuad[1] - 0x0BCA;		pCID[1] = cid_1;		pCID[2] = lcCID_ா;		cntCID = 3;		pCntUnicode -= 2;
					}
					else if (isconsonant && isVowelSign_6) {// 	ௌ
						pCID[0] = lcCID_ெ;									pCID[1] = cid_1;		pCID[2] = lcCID_ௗ;	cntCID = 3;		pCntUnicode -= 2;
					}
					else {
						pCID[0] = cid_1;				cntCID = 1;			pCntUnicode -= 1;
					}
					break;
				}
				case EMyFont::eLatha_Regular:				// Microsoft's Tamil Font (Regular)
				case EMyFont::eLatha_Bold:					// Microsoft's Tamil Font (Bold)
				default:
				{	
					const uint16_t lcOffset_BasicLatin = 267;											// Basic Latin Offset w.r.t. aLathaTamilMap.
					const uint16_t lcOffset_Latin_1_Supplement = 395;									// Latin-1 Supplement Offset w.r.t. aLathaTamilMap.
					const uint16_t lcOffset_Latin_Extended_A = 523;										// Latin Extended-A Offset w.r.t. aLathaTamilMap.
					const uint16_t lcOffset_General_Punctuation = 651;									// General Punctuation Offset w.r.t. aLathaTamilMap.
					const uint16_t lcOffset_Currency_Symbols = 763;										// Currency Symbols Offset w.r.t. aLathaTamilMap.
					const uint16_t lcOffset_Tamil_Ligature=128;											// Tamil_Ligature Characters Offset w.r.t. aLathaTamilMap.
					const uint16_t lcOffset_கி = lcOffset_Tamil_Ligature + 32;							// Offset w.r.t. aLathaTamilMap.
					const uint16_t lcCID_க = 66;														// CID for glyph க (U+0B95).
					const uint16_t lcCID_க் = 116;														// CID for glyph க்.
					const uint16_t lcCID_க்ஷ = 113;														// CID for glyph க்ஷ.
					const uint16_t lcCID_ஸ்ரீ = 188;														// CID for glyph ஸ்ரீ.
					const uint16_t lcCID_ா = 88;														// CID for glyph ா (U+0BBE)
					const uint16_t lcCID_ௗ = 100;														// CID for glyph ௗ (U+0BD7)
					const uint16_t lcCID_ெ = 93;														// CID for glyph ெ (U+0BC6)
					const uint16_t lcRangeOffset_1 = pUnicodeQuad[0] - aUnicode[(int)pLan].blockBeg;	// CID corresponding to unicode point pUnicodeQuad[0].
					const uint16_t lcRangeOffset_2 = pCntUnicode > 1 ? pUnicodeQuad[1] - aUnicode[(int)pLan].blockBeg : 0;	// CID corresponding to unicode point pUnicodeQuad[1].
					const uint16_t cid_1 = aLathaTamilMap[lcRangeOffset_1];								// CID corresponding to unicode point pUnicodeQuad[0].
					const uint16_t cid_2 = aLathaTamilMap[lcRangeOffset_2];								// CID corresponding to unicode point pUnicodeQuad[1].
					const bool isBasicLatin = pUnicodeQuad[0] >= aUnicode[(int)ELang::eEng].blockBeg && pUnicodeQuad[0] < aUnicode[(int)ELang::eEng].blockEnd/2;	// Basic Latin
					const bool isLatin_1_Supplement = pUnicodeQuad[0] >= 0x00A0 && pUnicodeQuad[0] <= 0x00FF;	// Latin-1 Supplement
					const bool isLatin_Extended_A = pUnicodeQuad[0] >= 0x0100 && pUnicodeQuad[0] <= 0x017F;		// Latin Extended-A
					const bool isGeneralPunctuation = pUnicodeQuad[0] >= 0x2000 && pUnicodeQuad[0] <= 0x206F;	// General Punctuation Block: U+2000 to U+206F
					const bool isCurrencySymbol = pUnicodeQuad[0] >= 0x20A0 && pUnicodeQuad[0] <= 0x20CF;		// Currency Symbols Block: U+20A0 to U+20CF
					const bool isக்ஷ = pCntUnicode > 2 && pUnicodeQuad[0] == 0x0B95 && pUnicodeQuad[1] == 0x0BCD && pUnicodeQuad[2] == 0x0BB7;	// க்ஷ Aksharam
					const bool isஸ்ரீ = pCntUnicode == 4 && pUnicodeQuad[0] == 0x0BB8 && pUnicodeQuad[1] == 0x0BCD && pUnicodeQuad[2] == 0x0BB0 && pUnicodeQuad[3] == 0x0BC0;	// ஸ்ரீ Aksharam
					const bool isconsonant = pUnicodeQuad[0] >= 0x0B95 && pUnicodeQuad[0] <= 0x0BB9;	// between க and ஹ
					const bool isVowelSign_1 = pCntUnicode > 1 && pUnicodeQuad[1] == 0x0BCD;								// Consonant ending with ்.
					const bool isVowelSign_2 = pCntUnicode > 1 && pUnicodeQuad[1] == 0x0BBE;								// Consonant ending with ா.
					const bool isVowelSign_3 = pCntUnicode > 1 && pUnicodeQuad[1] >= 0x0BBF && pUnicodeQuad[1] <= 0x0BC2;	// Consonant ending with ி,	ீ,	ு 	and ூ
					const bool isVowelSign_4 = pCntUnicode > 1 && pUnicodeQuad[1] >= 0x0BC6 && pUnicodeQuad[1] <= 0x0BC8;	// Consonant ending with ெ, 	ே and ை
					const bool isVowelSign_5 = pCntUnicode > 1 && pUnicodeQuad[1] >= 0x0BCA && pUnicodeQuad[1] <= 0x0BCB;	// Consonant ending with ொ,and ோ 
					const bool isVowelSign_6 = pCntUnicode > 1 && pUnicodeQuad[1] == 0x0BCC;								// Consonant ending with ௌ
					printf("isVowelSign_5=%d\n", isVowelSign_5);
					for (short kk = 0; kk < pCntUnicode; kk++) { printf("\tuni[%d]=0X%04x", kk, pUnicodeQuad[kk]);}  printf("\n"); //getchar();
					if (isBasicLatin) {// Basic Latin Block is from U+0000 to U+00FF, but allowed range is from U+0000 to U+0080.
						const uint16_t offset = pUnicodeQuad[0] - aUnicode[(int)ELang::eEng].blockBeg;
						pCID[0] = aLathaTamilMap[lcOffset_BasicLatin + offset];				cntCID = 1;				pCntUnicode -= 1;
					}
					else if (isLatin_1_Supplement) {// Latin-1 Supplement Block is from U+0080 to U+00FF.
						const uint16_t offset = pUnicodeQuad[0] - 0x0080;
						pCID[0] = aLathaTamilMap[lcOffset_Latin_1_Supplement + offset];		cntCID = 1;				pCntUnicode -= 1;
					}
					else if (isLatin_Extended_A) {// Latin Extended-A Block is from U+0100 to U+017F.
						const uint16_t offset = pUnicodeQuad[0] - 0x0100;
						pCID[0] = aLathaTamilMap[lcOffset_Latin_Extended_A + offset];		cntCID = 1;				pCntUnicode -= 1;
					}
					else if (isGeneralPunctuation) {// General Punctuation Block: U+2000 to U+206F.
						const uint16_t offset = pUnicodeQuad[0] - 0x2000;
						pCID[0] = aLathaTamilMap[lcOffset_General_Punctuation + offset];		cntCID = 1;			pCntUnicode -= 1;
					}
					else if (isCurrencySymbol) {// Currency Symbols Block: U+20A0 to U+20CF.
						const uint16_t offset = pUnicodeQuad[0] - 0x20A0;
						pCID[0] = aLathaTamilMap[lcOffset_Currency_Symbols + offset];		cntCID = 1;				pCntUnicode -= 1;
					}
					else if (isக்ஷ) {// க்ஷ
						pCID[0] = lcCID_க்ஷ;		cntCID = 1;				pCntUnicode -= 3;
					}
					else if (isஸ்ரீ) {// ஸ்ரீ
						pCID[0] = lcCID_ஸ்ரீ;			cntCID = 1;				pCntUnicode -= 4;
					}
					else if (isconsonant && isVowelSign_1) {// Pulli - ்
						const short offsetFrom_க = cid_1 - lcCID_க;			// offset w.r.t letter க.
						pCID[0] = lcCID_க் + offsetFrom_க;					cntCID = 1;		pCntUnicode -= 2;
					}
					else if (isconsonant && isVowelSign_2) {// Kaal - ா
						pCID[0] = cid_1;		pCID[1] = cid_2;			cntCID = 2;		pCntUnicode -= 2;
					}
					else if (isconsonant && isVowelSign_3) {//	ி,	ீ,	ு 	and ூ
						const short idx = pUnicodeQuad[1] - 0x0BBF;			//	ி -> 0x0BBF
						const short offsetFrom_க = cid_1 - lcCID_க;			// offset w.r.t letter க.
						const short offset = offsetFrom_க * 4 + idx + lcOffset_கி;
						//printf("idx=%d offsetFrom=%d offset=%d\n", idx, offsetFrom_க, offset);
						if (aLathaTamilMap[offset]) {
							pCID[0] = aLathaTamilMap[offset];		cntCID = 1;
						}
						else {
							pCID[0] = cid_1;		pCID[1] = cid_2;		cntCID = 2;
						}
						pCntUnicode -= 2;
					}
					else if (isconsonant && isVowelSign_4) {// 	ெ,	ே and ை
						pCID[0] = lcCID_ெ + pUnicodeQuad[1] - 0x0BC6;		pCID[1] = cid_1;		cntCID = 2;		pCntUnicode -= 2;
					}
					else if (isconsonant && isVowelSign_5) {// 	ொ and ோ
						pCID[0] = lcCID_ெ + pUnicodeQuad[1] - 0x0BCA;		pCID[1] = cid_1;		pCID[2] = lcCID_ா;		cntCID = 3;		pCntUnicode -= 2;
					}
					else if (isconsonant && isVowelSign_6) {// 	ௌ
						pCID[0] = lcCID_ெ;									pCID[1] = cid_1;		pCID[2] = lcCID_ௗ;	cntCID = 3;		pCntUnicode -= 2;
					}
					else {
						pCID[0] = cid_1;				cntCID = 1;			pCntUnicode -= 1;
					}
					break;
				}
			}	// end of switch statement.
			break;
		}
		case ELang::eHindi:					//  3. Hindi language
		{
			break;
		}
		case ELang::eMalayalam:				//  4. Malayalam language
		{
			break;
		}
		case ELang::eTelugu:				//  5. Telugu language
		{
			break;
		}
		case ELang::eKannada:				//  6. Kannada language
		{
			break;
		}
		case ELang::eMarathi:				//  7. Marathi language
		{
			break;
		}
		case ELang::eGujarati:				//  8. Gujarati language
		{
			break;
		}
		case ELang::eOdia:					//  9. Odia language
		{
			break;
		}
		case ELang::ePunjabi:				//  10. Punjabi language
		{
			break;
		}
		case ELang::eBengali:				//  11. Bengali language
		{
			break;
		}
		case ELang::eAssamese:				//  12. Assamese language
		default:
		{
			break;
		}
	}
	return cntCID;
}
char* strps(const ELang pLan, const EMyFont pMyFont, const char* pUTF8InString, char* pPSOutString, const unsigned short pPSOutputStringSize)		// convert utf-8 string to Postscript character code string.
{
	//
	// This functions converts utf-8 encoded character to Postscript characater code whose range will be between 128 to 255.
	// 
	// 1) pLan is an input parameter of type ELang supplying language of the Font.
	// 2) pMyFont is an input parameter of type EMyFont indicating name of the font.
	//    For example, Noto font, Vijaya font, Nirala font, etc.
	// 3) pUTF8InString is an input array parameter of type char that carries UTF-8 characters terminated with NULL byte.
	// 4) pPSOutString is an output array parameter of type char that passes out Postscript chracter Code corresponding to UTF-8 characters.
	//    This will be terminated with a NULL byte finally.
	// 5) pPSCharSize is an input parameter indicating the size of array parameter pPSOutString.
	//    This includes space for terminating NULL byte.
	// 
	// This function returns pointer to pPSOutString array.
	//

	pPSOutString[0] = 0;			// terminate with a null character.
	if (pLan < ELang::eEnglish || pLan > aLanguage[cMaxLanguage]) {
		return pPSOutString;		// Return NULL terminated string.
	}
	if (pLan == ELang::eEnglish) {
		for (short ii = 0, len=0; ii < (short)strlen(pUTF8InString); ii++) {
			sprintf_s(pPSOutString+len, pPSOutputStringSize-len, "%02x", pUTF8InString[ii] & 0xff);		len = (short)strlen(pPSOutString);
		}
		return pPSOutString;								// return string which is in hex format.
	}
	size_t len = 0, total = strlen(pUTF8InString);			// get total characters in the input parameter string pUTF8InString.
	uint32_t unicodeQuad[4] = { 0 };						// Array of type integer to store atmost 4 consecutive unicode points.
	short cntUnicode = 0;									// total number unicode points present in the array unicodeQuad.
	for (size_t ii = 0; ii < total; ii++) {
		unsigned int unicode;								// store unicode.
		short bytes=0;										// Number of bytes required for UTF-8 encoding.
		int ch = static_cast<unsigned char>(pUTF8InString[ii]);	// get next byte.
		if (ch >= 0 && ch <= 127) unicode = ch;				// U-00000000 - U-0000007F: 0xxxxxxx												utf-8 1 byte encoding
		else if ((ch & 0xE0) == 0xC0) {						// U-00000080 - U-000007FF: 110xxxxx 10xxxxxx										utf-8 2 byte encoding
			unicode = 0x1f & ch;							// Extract 5 bits 
			bytes = 1;										// 1 byte is already extracted and 1 bytes to further process.
		}
		else if ((ch & 0xF0) == 0xE0) {						// U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx								utf-8 3 byte encoding
			unicode = 0x0f & ch;							// Extract 4 bits 
			bytes = 2;										// 1 byte is already extracted and 2 bytes to further process.
		}
		else if ((ch & 0xF8) == 0xF0) {						// U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx						utf-8 4 byte encoding
			unicode = 0x07 & ch;							// Extract 3 bits 
			bytes = 3;										// 1 byte is already extracted and 3 bytes to further process.
		}
		else if ((ch & 0xFC) == 0xF8) {						// U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx			utf-8 5 byte encoding
			unicode = 0x03 & ch;							// Extract 2 bits 
			bytes = 4;										// 1 byte is already extracted and 4 bytes to further process.
		}
		else if ((ch & 0xFE) == 0xFC) {						// U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx	utf-8 6 byte encoding
			unicode = 0x01 & ch;							// Extract 1 bits 
			bytes = 5;										// 1 byte is already extracted and 5 bytes to further process.		
		}
		else break;											// An invalid utf8 encountered. Stop processing and returns already processed string.
		// Process Multi-byte UTF-8.
		short kk = 0;
		while (++kk <= bytes) {												// This logic will work only for UTF-8 encoding with atleast 2 bytes.
			int nextCh = static_cast<unsigned char>(pUTF8InString[ii+kk]);	// get next byte.
			unicode = (unicode << 6) | (0x3f & nextCh);						// Extract from multi-byte UTF-8.
		}
		ii += bytes;														// Next UTF-8 to process.
		unicodeQuad[cntUnicode] = unicode;									// store unicode point.
		if (++cntUnicode < 4 && (ii+1) < total) continue;					// Generate atmost 4 consecutive unicode points.
		uint16_t cid[3] = { 0 };											// Maximum 3 CIDs needed for ligatures or conjuncts.
	Label_Repeat:
		//printf("before: cnt=%d\n", cntUnicode);
		const short cntUnicodeSave = cntUnicode;							// Take a copy.
		const short cntCID = u2ps(pLan, pMyFont, unicodeQuad, cntUnicode, cid);	// Count of CIDs present in the cid array. This value could be either 1, 2, 3 or 4.
		if (!cntCID) break;													// Error. Mostly pFontName does not match with registered font names.
		for (short kk = 0; kk < cntCID; kk++) {
			if ((len + 2) >= pPSOutputStringSize) break;					// Insufficient pPSOutputStringSize to store character code. Stop processing and return already processed string.
			sprintf_s(pPSOutString+len, pPSOutputStringSize-len, "%04x", cid[kk]);		len = strlen(pPSOutString);
		}
		//printf("after: cnt=%d\n", cntUnicode);
		if (cntUnicode) {
			const short consumedUnicode = cntUnicodeSave - cntUnicode;		// Number of unicode points consumed.
			for (short kk = 0; kk < cntUnicode; kk++) {
				unicodeQuad[kk] = unicodeQuad[kk+consumedUnicode];			// Store Only unicode points which are not consumed.
			}
		}
		if (ii + 1 == total && cntUnicode) goto Label_Repeat;
		//printf("%2d) 0X%04x %4d %3u\n", (short)ii, unicode, unicode , *(str - 1) & 0xff);
	}
	return pPSOutString;
}
const char *getCIDfilenameWithoutPath(const char *pCIDfilename)
{
	short len = static_cast<short>(strlen(pCIDfilename));
	while (pCIDfilename[--len] && pCIDfilename[len] != '/' && pCIDfilename[len] != '\\');
	return len ? pCIDfilename + len + 1 : pCIDfilename;
}
int main(int argc, char* argv[])
{
    //
    // utf8map filename.t42
    //
    if (argc == 1 || argc > 2) {
#if _MSC_VER			// Visual Studio
        fprintf(stdout, "usage: utf8map filename.t42");
#elif __GNUC__	|| __CYGWIN__		// gcc
        fprintf(stdout, "usage: ./utf8map filename.t42");
#endif
        printf("\nhit any key....");	getchar();
        return(1);				// exit with error 1
    }
    const char* ptr;
    const char* strCIDFontFile = argv[1];       // ttf filename
    if (!(ptr = strstr(strCIDFontFile, ".t42"))) {
        fprintf(stdout, "Input file '%s' does not have file extension 't42'.", strCIDFontFile);
        printf("\nhit any key....");	getchar();
        return(1);				// exit with error 1
    }
	const char *cidFilenameNoPath = getCIDfilenameWithoutPath(strCIDFontFile);
	// find out the language and font 

	ELang lan=ELang::eZero;								// Language of the cid-keyed font passed as argument.
	EMyFont myfont= EMyFont::eZero;						// Font name.
	short ii = static_cast<short>(ELang::eTamil);		// start from Tamil whose index is 2.
	do {
		short jj = 0;
		while (asMyFont[ii][++jj].numGlyphs) {
			if (!_stricmp(asMyFont[ii][jj].fname, cidFilenameNoPath)) {
				myfont = static_cast<EMyFont>(jj);
				lan = static_cast<ELang>(ii);
			}
		}
	} while (++ii <= cMaxLanguage);

	if (lan == ELang::eZero || myfont == EMyFont::eZero) {
		fprintf(stdout, "Font file '%s' is not found in the CID-Keyed font list.", strCIDFontFile);
		printf("\nhit any key....");	getchar();
		return(1);				// exit with error 1
	}
    const short lcFileNameSize = 256;
    char psFilename[lcFileNameSize];
    strcpy_s(psFilename, lcFileNameSize, strCIDFontFile);              // copy t42 file name
    char* ptr2 = strstr(psFilename, ".t42");            memcpy(static_cast<void*>(ptr2 + 1), "ps", 3);       // replace ttf with ps.
    //printf("%s...\t%s...\n", t42Filenamet, psFilename); getchar();

    FILE *fcid, *fps;

#if _MSC_VER			// Visual Studio
    // open t42 file to read
    if (fopen_s(&fcid, strCIDFontFile, "r")) {
        printf("File name: %s\n", strCIDFontFile);
        perror("The following error occurred");
        return (1);
    }
    else printf("File %s is opened for reading\n", strCIDFontFile);
    // open ps file to write
    if (fopen_s(&fps, psFilename, "w")) {
        printf("File name: %s\n", psFilename);
        perror("The following error occurred");
        return (1);
    }
    else printf("File %s is opened for writing\n", psFilename);

#elif __GNUC__	|| __CYGWIN__		// gcc
    // open t42 file to read
    if (!(fcid = fopen(strCIDFontFile, "r"))) {
        printf("File name: %s\n", strCIDFontFile);
        perror("The following error occurred");
        return (1);
    }
    else printf("File %s is opened for writing\n", strCIDFontFile);
    // open ps file to write
    if (!(fps = fopen(psFilename, "w"))) {
        printf("File name: %s\n", psFilename);
        perror("The following error occurred");
        return (1);
    }
    else printf("File %s is opened for writing\n", psFilename);

#endif
}

