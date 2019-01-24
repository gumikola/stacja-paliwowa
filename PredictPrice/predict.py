import os
import sqlite3

import pandas as pd
from fbprophet import Prophet

PATH = "/home/mguz/workspace/stacja-paliwowa/FuelPricesChartData"
TYPES_FUEL = ("ON", "PB95", "OO", "PB98")
PROVIDERS = ("Lotos", "Orlen")
period_future = 60
conn = sqlite3.connect('/home/mguz/workspace/stacja-paliwowa/sqlite.db')


def dateparse(x): return pd.datetime.strptime(x, '%d-%m-%Y')


for provider in PROVIDERS:
    for type_fuel in TYPES_FUEL:
        # series = pd.read_csv(PATH + t + ".csv", header=None, names=[
        #                      "ds", "y"], index_col=0, parse_dates=True, date_parser=dateparse)
        q = "SELECT Data,Cena FROM Ceny_paliwa where Typ_paliwa_Nazwa='{}' and Rafineria='{}'".format(
            type_fuel, provider)
        print(q)
        series = pd.read_sql_query(q, conn, parse_dates={'Data': '%d-%m-%Y'}).rename({
            'Data': 'ds', 'Cena': 'y'}, axis='columns').set_index('ds')
        print(series)

        series = series.resample('D').ffill().reset_index()
        print(series)

        model = Prophet(changepoint_prior_scale=0.05, yearly_seasonality=15)
        model.fit(series)
        future = model.make_future_dataframe(periods=period_future)
        forecast = model.predict(future)

        data_to_save = forecast[["ds", "yhat"]][-period_future:]

        print(data_to_save)

        with open("{}/{}/{}{}".format(PATH, provider, type_fuel, ".csv"), 'w') as csvfile:

            series.to_csv(csvfile, header=False, index=False,
                          sep=',', float_format='%1.0f', date_format='%d-%m-%Y')
            data_to_save.to_csv(csvfile, header=False, index=False,
                                sep=',', float_format='%1.0f', date_format='%d-%m-%Y')
