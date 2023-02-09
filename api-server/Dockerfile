FROM python:3.9

WORKDIR /app/

COPY ./app.py /app/
COPY ./utils/model.py /app/utils/
COPY ./utils/utils.py /app/utils/
COPY ./requirements.txt /app/
COPY ./dbutils/database.py /app/dbutils
COPY ./dbutils/awsconfig.py /app/dbutils/awsconfig.py
COPY ./dbutils/query.py /app/dbutils/query.py


RUN pip3 install -r requirements.txt

CMD uvicorn app:app --host=0.0.0.0 --reload