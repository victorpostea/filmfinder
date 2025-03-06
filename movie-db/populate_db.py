import mysql.connector
import random
from datetime import datetime, timedelta

# Connect to MySQL
conn = mysql.connector.connect(
    host="movie-db",
    user="root",
    password="mypassword",
    database="MovieDB"
)
cursor = conn.cursor()

# Sample data
movies = [
    ("Inception", 8.8, "Sci-Fi"),
    ("The Dark Knight", 9.0, "Action"),
    ("Forrest Gump", 8.8, "Drama"),
    ("Interstellar", 8.6, "Sci-Fi"),
    ("Titanic", 7.8, "Romance"),
    ("The Matrix", 8.7, "Sci-Fi")
]

# Insert random movies with random dates
for title, rating, genre in movies:
    release_date = datetime(1990, 1, 1) + timedelta(days=random.randint(0, 10000))
    cursor.execute("INSERT INTO Movies (title, rating, genre, releaseDate) VALUES (%s, %s, %s, %s)",
                   (title, rating, genre, release_date.strftime('%Y-%m-%d')))

# Commit and close
conn.commit()
cursor.close()
conn.close()
print("Database populated with movies.")