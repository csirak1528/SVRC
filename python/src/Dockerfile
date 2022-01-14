FROM python:3.9
WORKDIR python
COPY requirements.txt .
RUN pip install --no-cache-dir --upgrade -r /web10/requirements.txt
COPY . /python 
CMD ["python", "main.py"]