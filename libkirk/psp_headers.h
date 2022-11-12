#pragma pack(push)
#pragma pack(1)

typedef struct 
{ 
	u32 e_magic;
	u8	e_class;
	u8	e_data;
	u8	e_idver;
	u8	e_pad[9];
	u16 e_type; 
    u16 e_machine; 
    u32 e_version; 
    u32 e_entry; 
    u32 e_phoff; 
    u32 e_shoff; 
    u32 e_flags; 
    u16 e_ehsize; 
    u16 e_phentsize; 
    u16 e_phnum; 
    u16 e_shentsize; 
    u16 e_shnum; 
    u16 e_shstrndx; 
} Elf32_Ehdr;// __attribute__((packed)) Elf32_Ehdr;

typedef struct 
{ 
	u32 p_type; 
	u32 p_offset; 
	u32	p_vaddr; 
	u32	p_paddr; 
    u32	p_filesz; 
    u32	p_memsz; 
    u32	p_flags; 
    u32 p_align; 
} /*__attribute__((packed)) */ Elf32_Phdr;

typedef struct 
{ 
	u32 sh_name; 
	u32 sh_type; 
	u32 sh_flags; 
	u32 sh_addr; 
	u32 sh_offset; 
	u32 sh_size; 
	u32 sh_link; 
	u32 sh_info; 
	u32 sh_addralign; 
	u32 sh_entsize; 
} /*__attribute__((packed)) */ Elf32_Shdr;

typedef struct {
	u32	r_offset;
	u32	r_info;		/* sym, type: ELF32_R_... */
} Elf32_Rel;


typedef struct 
{
	u16		attribute;
	u8		module_ver_lo;	
	u8		module_ver_hi;
	char	modname[28];
} /* __attribute__((packed)) */ PspModuleInfo;

typedef struct {
    u16  modattribute;
    u8   modversion[2]; /* minor, major, etc... */
    char modname[28];
    void *gp_value;
    void *ent_top;
    void *ent_end;
    void *stub_top;
    void *stub_end;
}SceModuleInfo;

typedef struct
{
    u32 signature;          //0 "~PSP"
    u16 mod_attribute;      //4 $0000 | $0200
    u16 comp_attribute;     //6 $0000 | $0000 compress method:
							//        0x0001=PRX Compress
							//        0x0002=ELF Packed
							//        0x0008=GZIP overlap
							//        0x0200=KL4E(if not set, GZIP)

    u8 module_ver_lo;       //8 01 | 01
    u8 module_ver_hi;       //9 01 | 01
    char modname[28];       //0xA "Intro" | "LocoRoco"
    u8 mod_version;         //0x26 01 | 01
    u8 nsegments;           //0x27 01 | 01
    u32 elf_size;           //0x28 $000100ce (65742) | $007ee686
    u32 psp_size;           //0x2C $00010220 (66080) | $007ee7e0
    u32 boot_entry;         //0x30 $00000698 | $0002EFE0
    u32 modinfo_offset;     //0x34 $0000be80 | $0001DC90
    int bss_size;           //0x38 $000520d8 | $001ab3d0
    u16 seg_align[4];       //0x3C $0010 $0000 $0000 $0000 | $0010 $0000 $0000 $0000
    u32 seg_address[4];     //0x44 $00000000 $00000000 $00000000 $00000000 | Same
    int seg_size[4];        //0x54 $0005f758 $00000000 $00000000 $00000000 | $0093BF80 $00000000 $00000000 $00000000
    u32 reserved[5];        //0x64 $00000000 $00000000 $00000000 $00000000 $00000000 | Same
    u32 devkit_version;     //0x78 $06020010 | $00000000
    u8 decrypt_mode;        //0x7C 9 | $D
    u8 padding;             //0x7D 0 | 0
    u16 overlap_size;       //0x7E $0000 | $0000
    u8 key_data[0x30];      //0x80 9CB34C3C81F30E1553C41302E7DCE86826903AE1D08DFC27AA0634A89B75833F0BAB03B3C2D4FBF776B71C48C8C170E3 |
                            //     0ABCCEE6D42F6F38FE0766DD6732676365CA85163C2402FFC5DD9A6714D0F920B3BDD7435065A7A0213B23ACD28D6AD3
    u32 comp_size;          //0xB0 $000100CE | $007EE686
    int _80;                //0xB4 $80 | $80
    u32 unk_B8;             //0xB8 $00000000 | $00000000
    u32 unk_BC;             //0xBC $00000000 | $00000000
    u8 key_data2[0x10];     //0xC0 11F5F17C4B2E591BAD3F7019081F12F9 | B0D8365A43F1ED8599F93C0E7D6290CF
    u32 tag;                //0xD0 8004FD03 | ADF305F0
    u8 scheck[0x58];        //0xD4 All 0 | Same
    u8 sha1_hash[0x14];     //0x12C 28BEBDF5DCAE4A864FC67A56E21F3E2E4157A6CA | 9B0F78565199310722307B572818917E28BFE040
    u8 key_data4[0x10];     //0x140 64E61F0703DD654B797DD18F2B4A3131 | 39CD7B8538C211B9203EC9B3A6AC4451
} PSP_Header2; //0x150


typedef struct
{
	u32		        signature;      // 0
	PspModuleInfo   mod_info;       //4
	u8				version;        // 26
	u8				nsegments;      // 27
	int				elf_size;       // 28
	int				psp_size;       // 2C
	u32				entry;	        // 30
	u32				modinfo_offset; // 34
	int				bss_size;       // 38
	u16				seg_align[4];   // 3C
	u32				seg_address[4]; // 44
	int				seg_size[4];    // 54
	u32				reserved[5];    // 64
	u32				devkitversion;  // 78
	u32				decrypt_mode;   // 7C 
	u8				key_data0[0x30];// 80
	int				comp_size;      // B0
	int				_80;            // B4
	int				reserved2[2];   // B8
	u8				key_data1[0x10];// C0
	u32				tag;            // D0
	u8				scheck[0x58];   // D4
	u32				key_data2;      // 12C
	u32				oe_tag;         // 130
	u8				key_data3[0x1C];// 134
} /*__attribute__((packed))*/ PSP_Header;

typedef struct {
   int id; // 0x5053507E
   int unk1; // run flags?
   short version;
   char name[28];
   u8 unk2; // 0x01
   u8 e_phnum;
   int datasize; // filesize - 0x150
   int filesize; // filesize
   int e_entry;
   int modinfo_offset;
   int bss_size;
   short p_align[4];
   int p_vaddr[4];
   int p_memsz[4];
   int unk3[6];
   int type;
   u8 data3[0x30];
   u8 data5[0x10]; // kirk head part, 0x70
   u8 data4[0x10];
   int tag;
   u8 empty[0x58];
   u8 data2[0x14];
   u8 data1[0x10];
} psphead_t;


#pragma pack(pop)
