import { Injectable } from '@angular/core';
import { HttpClient } from  '@angular/common/http';
import { Movie, MovieResponse } from './models/movie.model';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class MovieService {
  private url = 'http://localhost:8080';
  private searchExtension = '/movies?query='

  constructor(private http: HttpClient) {}

  searchMovie(query: String): Observable<MovieResponse> {
    return this.http.get<MovieResponse>(`${this.url}${this.searchExtension}${query}`)
  }

}
