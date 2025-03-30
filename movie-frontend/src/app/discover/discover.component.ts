import { Component } from '@angular/core';
import { MovieService } from '../movie.service';
import { Movie, MovieResponse } from '../models/movie.model';

@Component({
  selector: 'app-discover',
  templateUrl: './discover.component.html',
  styleUrls: ['./discover.component.css']
})
export class DiscoverComponent {
  selectedGenre: string = '';
  selectedYear: number | null = null;
  selectedRating: number | null = null;
  movies: Movie[] = [];
  selectedSort: string = '';
  totalResults: number = 0;
  currentPage: number = 1;

  constructor(private movieservice: MovieService) {}

  discoverFilms() {
    if (this.selectedGenre) {
      console.log("Discovering movies for genre:", this.selectedGenre);
      this.currentPage = 1;
      this.movies = [];
  
      this.movieservice
        .discoverMovies(this.selectedGenre, this.currentPage, this.selectedYear, this.selectedRating, this.selectedSort)
        .subscribe(response => {
          const parsed = this.parseData(response);
          this.movies = parsed.results;
          this.totalResults = parsed.total_results || parsed.results.length;
        });
    }
  }
  
  loadNextPage() {
    this.currentPage++;
  
    this.movieservice
      .discoverMovies(this.selectedGenre, this.currentPage, this.selectedYear, this.selectedRating, this.selectedSort)
      .subscribe(response => {
        const parsed = this.parseData(response);
        this.movies = [...this.movies, ...parsed.results];
      });
  }  

  parseData(response: MovieResponse): MovieResponse {
    if (!response.results) {
      response.results = [];
      return response;
    }

    response.results = response.results.map(movie => ({
      ...movie,
      title: movie.title || "Unknown Title",
      release_date: movie.release_date || "N/A",
      vote_average: typeof movie.vote_average === "number" ? movie.vote_average : 0,
      overview: movie.overview || "No overview available.",
      poster_path: movie.poster_path
        ? `https://image.tmdb.org/t/p/w500${movie.poster_path}`
        : "/assets/img_gone.png"
    }));

    return response;
  }
}