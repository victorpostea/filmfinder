FROM python:3.9

WORKDIR /app

COPY populate_db.py .
RUN pip install mysql-connector-python

CMD ["python3", "populate_db.py"]