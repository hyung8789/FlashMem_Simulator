# FlashMem_Simulator

|명령어|의미|
|---|:---:|
|init x or i x | Create x MB Storage File |
|read PSN or r PSN | Read data at Physical Sector Num(PSN) Position |
|write PSN data or w PSN data | Write data at Physical Sector Num(PSN) Position |
|erase PBN or e PBN | Erase data at Physical Block Num(PBN) Position |
<br>

- Simulator that creates flash memory, reads data in physical sectors, inputs data, and erases data in blocks
- No limit on FlashMem Storage File creation capacity<br>
- Unable to overwrite the same physical sector location<br>
