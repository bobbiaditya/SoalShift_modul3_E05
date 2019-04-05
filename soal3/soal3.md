### Nomor 3
Pada soal nomor 3 kita diberikan sebuah permasalahan tentang Agmal dan Iraj. Agmal memiliki `WakeUp_Status`yang awalnya 0 dan Iraj memiliki `Spirit_Status` yang awalnya 100. Agmal bisa menambah `WakeUp_Status` nya , Iraj bisa mengurangi `Spirit_Status` nya.

Fitur:
- All Status, yaitu menampilkan status kedua sahabat
Ex: Agmal WakeUp_Status = 75
Iraj Spirit_Status = 30
- “Agmal Ayo Bangun” menambah WakeUp_Status Agmal sebesar
15 point
- “Iraj Ayo Tidur” mengurangi Spirit_Status Iraj sebanyak 20 point

Kondisi:
- Jika Fitur “Agmal Ayo Bangun” dijalankan sebanyak 3 kali, maka
Fitur “Iraj Ayo Tidur” Tidak bisa dijalankan selama 10 detik
(Dengan mengirim pesan ke sistem “Fitur Iraj Ayo Tidur disabled
10 s”)
- Jika Fitur “Iraj Ayo Tidur” dijalankan sebanyak 3 kali, maka Fitur
“Agmal Ayo Bangun” Tidak bisa dijalankan selama 10 detik
(Dengan mengirim pesan ke sistem “Agmal Ayo Bangun disabled
10 s”)

Program akan berhenti jika Salah Satu :
- WakeUp_Status Agmal >= 100 (Tampilkan Pesan “Agmal
Terbangun,mereka bangun pagi dan berolahraga”)
- Spirit_Status Iraj <= 0 (Tampilkan Pesan “Iraj ikut tidur, dan
bangun kesiangan bersama Agmal”)


