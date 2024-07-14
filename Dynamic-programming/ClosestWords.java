/*
* Authors: Farhan Syed & Abhinav Sasikumar
* Year: 2023
*/
import java.util.LinkedList;
import java.util.List;
import java.util.*;

public class ClosestWords {
  LinkedList<String> closestWords = null;
  int dynProgMatrix[][] = new int[41][41];    // Matrix that stores previously calculated distances
  String previousWord = "";

  int closestDistance = -1;

  int partDist(String w1, String w2, int w1len, int w2len) {
    int numberOfMatchingLetters = 0;
    int minLength = Math.min(previousWord.length(), w2.length()); // Gets minimum length of previous word and new word. 

    // Counts how many letters match in previous and new word.  
    for (int i = 0; i < minLength ; i++) {
      if (w2.charAt(i) == previousWord.charAt(i)) {
        numberOfMatchingLetters++;
      }
      else
        break;
    }

    // If new word is longer than previous word, we must fill the first row and first column with i
    if(previousWord.length() < w2len){
      // First column 
      for(int i = previousWord.length(); i <= w1len; i++){
        dynProgMatrix[i][0] = i;
      }
      // First row 
      for(int i = previousWord.length();i <= w2len; i++){
        dynProgMatrix[0][i] = i;
      }
    }
    
    // No match is equivalent to 1 match
    if(numberOfMatchingLetters == 0){
      numberOfMatchingLetters = 1;
    }

    // Go through each element in the matrix. 
    for (int i = 1; i <= w1len; i++) {
      for (int j = numberOfMatchingLetters; j <= w2len; j++) {       // We can skip the values that will be identical to matrix of previous word
        int costOfSub = 0;
        if (w1.charAt(i-1) == w2.charAt(j-1)) {
          costOfSub = 0;      // Same letter -> no edit -> no cost of substitution 
        }
        else {
          costOfSub = 1;    
          
        }
        int a = dynProgMatrix[i - 1][j - 1] + costOfSub; // Substitution
        int b = dynProgMatrix[i - 1][j] + 1;            // Remove
        int c = dynProgMatrix[i][j - 1] + 1;            // Add
        dynProgMatrix[i][j] = Math.min(Math.min(a, b), c);

      }
    }
    previousWord = w2;
    return dynProgMatrix[w1len][w2len];
  }

  int distance(String w1, String w2) {
    return partDist(w1, w2, w1.length(), w2.length());
  }

  public ClosestWords(String w, List<String> wordList) {
    for (String s : wordList) {

      int dist = distance(w, s);
      if (dist < closestDistance || closestDistance == -1) {
        closestDistance = dist;
        closestWords = new LinkedList<String>();
        closestWords.add(s);
      } else if (dist == closestDistance)
        closestWords.add(s);
    }

  }

  int getMinDistance() {
    return closestDistance;
  }

  List<String> getClosestWords() {
    return closestWords;
  }

}