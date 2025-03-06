# 🎬 FilmFinder - A Containerized Movie Database App

FilmFinder is a **Dockerized** movie database app that consists of:
- **movie-backend** → A C++ backend using Crow and MySQL
- **movie-frontend** → An Angular frontend
- **movie-db** → A MySQL database with preloaded movie data

---

## 📦 Requirements
To run FilmFinder, make sure you have:
- [Docker](https://www.docker.com/products/docker-desktop) installed and running.
- **Git** installed to manage submodules.

---

## 🚀 Setting Up the Project

### **1️⃣ Clone the Repository**
```sh
git clone --recursive <your-repo-url>
cd filmfinder
```
> If you've already cloned the repo **without** `--recursive`, run:
> ```sh
> git submodule update --init --recursive
> ```

---

### **2️⃣ Build and Run All Containers**
Run the following command in the **filmfinder/** directory:
```sh
docker-compose up -d --build
```
- This will:
  - **Build and start** `movie-db` (MySQL database).
  - **Initialize the database** with `init.sql`.
  - **Run `populate_db.py`** to insert sample movie data.
  - **Start `movie-backend`** (C++ Crow server).
  - **Launch `movie-frontend`** (Angular web app).

---

## 🔗 Access the App
After starting the containers, you can access:

| Service           | URL                           |
|------------------|------------------------------|
| **Frontend (Angular)**  | [http://localhost:4200](http://localhost:4200) |
| **Backend (Crow API)**  | [http://localhost:8080](http://localhost:8080) |
| **Database (MySQL)**  | Host: `localhost`, Port: `3306` |

---

## ⏹️ Stopping the Containers
To stop all running containers:
```sh
docker-compose down
```

---

## ⚙️ Troubleshooting
### **Submodules Not Found?**
If `external/crow` or `external/asio` is missing, run:
```sh
git submodule update --init --recursive
```

---

## 🤝 Contributing
1. Fork the repo
2. Create a new branch (`git checkout -b feature-name`)
3. Commit your changes (`git commit -m "Added new feature"`)
4. Push to your branch (`git push origin feature-name`)
5. Open a Pull Request

---

## 📜 License
This project is licensed under the MIT License.

---

🎬 **Enjoy FilmFinder!** 🚀
