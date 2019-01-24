import sqlite3
from datetime import datetime

import requests
from bs4 import BeautifulSoup

session = requests.Session()
session.headers.update(
    {'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36'})


def save_to_db(source, type_fuel, price):
    print(source,type_fuel, price)
    conn = sqlite3.connect('/home/mguz/workspace/stacja-paliwowa/sqlite.db')
    c = conn.cursor()
    c.execute("INSERT OR IGNORE INTO Ceny_paliwa(Typ_paliwa_Nazwa,Cena,Data,Rafineria) VALUES(?,?,?,?)",
              (type_fuel, price, datetime.now().strftime('%d-%m-%Y'), source))
    conn.commit()
    conn.close()


def lotos_get():
    type_fuel = {"Benzyna bezołowiowa 95": "PB95",
                 "Benzyna bezołowiowa 98": "PB98",
                 "Olej napędowy EURODIESEL": "ON",
                 "Olej napędowy do celów opałowych LOTOS RED": "OO"
                 }

    page = session.get(
        "http://www.lotos.pl/144/poznaj_lotos/dla_biznesu/hurtowe_ceny_paliw")
    soup = BeautifulSoup(page.content, 'html.parser')

    table = soup.find(class_='printable').find(
        class_="static-text").find('table')
    prices = iter(table.find_all('td'))

    for price in prices:
        try:
            save_to_db("Lotos", type_fuel[price.contents[0]], next(
                prices).contents[0].replace(' ', '').split(',')[0])
        except KeyError:
            pass


def orlen_get():
    type_fuel = {"Benzyna bezołowiowa - Eurosuper 95": "PB95",
                 "Benzyna bezołowiowa - Super Plus 98": "PB98",
                 "Olej Napędowy Ekodiesel": "ON",
                 "Olej Napędowy Grzewczy Ekoterm": "OO"
                 }

    page = session.get(
        "https://www.orlen.pl/PL/DlaBiznesu/HurtoweCenyPaliw/Strony/default.aspx")
    soup = BeautifulSoup(page.content, 'html.parser')

    table = soup.find(id='MSOZoneCell_WebPartctl00_ctl00_SPWebPartManager1_g_753cafe9_2be0_414b_aa26_6f746d63d018').find(
        class_="ms-WPBody").find("table")
    prices = iter(table.find_all('span'))

    for price in prices:
        try:
            str_type = type_fuel[price.contents[0].strip()]
            str_price = next(prices).contents[0].replace(
                '\xa0', '').split(',')[0]
            save_to_db("Orlen", str_type, str_price)
            next(prices).contents[0]  # skip trash from html
        except KeyError:
            pass


def bp_get():
    type_fuel = {"Benzyna bezołowiowa - Pb95": "PB95",
                 "Benzyna bezołowiowa - Pb98": "PB98",
                 "Olej Napędowy": "ON"
                 }

    page = session.get(
        "https://www.bp.com/pl_pl/bp-plus/poland/doswiadczenie-w-zakresie-paliw/hurtowa-sprzedaz-paliw-bp.html")
    soup = BeautifulSoup(page.content, 'html.parser')

    table = soup.find(class_="parsys_column cq-colctrl-lt0-c1").find("table")
    prices = iter(table.find_all("pre"))

    for price in prices:
        save_to_db("BP", type_fuel[price.contents[0].strip()],
                   next(prices).contents[0].split(' ')[0])


if __name__ == "__main__":
    bp_get()
    orlen_get()
    lotos_get()
