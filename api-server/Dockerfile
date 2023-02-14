FROM python:3.9

WORKDIR /app/

COPY ./app.py /app/
COPY ./utils/ /app/utils/
COPY ./requirements.txt /app/
COPY ./dbutils/ /app/dbutils


RUN pip3 install -r requirements.txt

CMD uvicorn app:app --host=0.0.0.0 --reload