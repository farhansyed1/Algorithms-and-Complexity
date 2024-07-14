/*
Lab 1: Concordance 

* Authors: Abhinav Sasikumar & Farhan Syed
* Year: 2023

*/
import java.util.*;
import java.nio.charset.StandardCharsets;
import java.io.*;

public class Konkordans {
    private File words; // Contains: All the words found in the korpus file; pointer to the first index
                        // position of the word in the indexes file; the word count
    private File indexes; // The index positions of each word in korpus
    private File hash; // The binary file containing the hashed words
    private File korpus; // The concordance

    public static void main(String[] args) throws Exception {

        Konkordans kon = new Konkordans();
        kon.words = new File("/var/tmp/words.txt"); // temporärarean /var/tmp
        kon.indexes = new File("/var/tmp/indexes.txt");
        kon.hash = new File("/var/tmp/hash.txt");
        kon.korpus = new File("/afs/kth.se/misc/info/kurser/DD2350/adk23/labb1/korpus");

        // If no word is given, construct the concordance
        if (args.length == 0) {
            kon.constructConcordance();
            kon.constructHash();
        }
        // Invalid argument is given
        else if (args.length != 1) {
            throw new Exception("Invalid argument");
        }

        // Search for the word in the concordance
        if (args.length != 0) {
            kon.searchConcordance(args[0].toLowerCase());
        }
    }

    /*
     * Looks for the specific word in the concordance and prints the sentences it is
     * found in.
     * 
     */
    private void searchConcordance(String searchTerm) throws FileNotFoundException {
        char[] hashString;
        RandomAccessFile hashFile = new RandomAccessFile(hash, "r");
        RandomAccessFile wordsFile = new RandomAccessFile(words, "r");
        RandomAccessFile indexFile = new RandomAccessFile(indexes, "r");
        RandomAccessFile korpusFile = new RandomAccessFile(korpus, "r");

        long startTime = System.currentTimeMillis(); // Timer for checking how long it takes to find the word

        // If the word is shorter than 3 letters -> we want to hash the whole word
        if (searchTerm.length() <= 3) {
            hashString = searchTerm.toCharArray();
        }
        // Longer than 3 letters -> we want to hash the first 3 letters
        else {
            hashString = searchTerm.substring(0, 3).toCharArray();
        }

        try {
            long hashValue = hashFunction(hashString); // The hash value of the word
            hashFile.seek(hashValue); // Go to the position in the hash file
            long prefixPointer = hashFile.readLong(); // Get the value of the prefix pointer to the selected word
            hashFile.seek(hashValue + 8);
            long nextPrefixPointer = hashFile.readLong();

            // Binary Search to find the word
            wordsFile.seek(prefixPointer);
            String[] finalLine = {};

            long firstIndex = prefixPointer;
            long secondIndex = nextPrefixPointer;
            boolean skip = false;

            if (secondIndex == 0) {
                secondIndex = wordsFile.length();

            }
            // Does binary search until the distance between the two indices is less than a
            // 1000.
            while (firstIndex + 1000 <= secondIndex && !skip) {
                long middleIndex = firstIndex + (secondIndex - firstIndex) / 2; // Finds the midpoint between the two
                                                                                // indices
                wordsFile.seek(middleIndex); // Seeks to the midpoint
                wordsFile.readLine(); // Move to the beginning of the line
                String nextLine = wordsFile.readLine();
                if (nextLine == null) {
                    break; // Exit loop if end of file is reached
                }
                String[] nextLineSplit = nextLine.trim().split(" ");
                String newWord = nextLineSplit[0];
                int comparison = newWord.compareTo(searchTerm); // Compares word to searchTerm.

                if (comparison == 0) { // If the word found is a match, then the loop is broken.
                    finalLine = nextLineSplit;
                    long endTime = System.currentTimeMillis();
                    long timeTaken = endTime - startTime;
                    System.out.println("Tid: " + timeTaken + " ms \n");
                    System.out.println("Found word");
                    break;
                } else if (comparison < 0) { // Otherwise sets the firstindex to the mid Index, meaning our word is
                                             // below the
                    // middleIndex
                    firstIndex = middleIndex;
                } else { // Otherwise sets the second index value to the middleIndex, as our word is
                         // between the first index and the middle index.
                    secondIndex = middleIndex;
                }
            }
            // If the word has not been found, we continue with a linear search
            wordsFile.seek(firstIndex); // We first seek to the initial index to begin our linear search at

            // Linear search
            while (!skip) {
                String nextLine = wordsFile.readLine();
                firstIndex++;
                if (nextLine == null) {
                    break; // Exit loop if end of file is reached
                }
                String[] nextLineSplit = nextLine.trim().split(" ");
                String newWord = nextLineSplit[0];

                if (newWord.equals(searchTerm)) { // Word is found, so we exit the search
                    finalLine = nextLineSplit;
                    break;
                } else if (firstIndex > secondIndex) {
                    long endTime = System.currentTimeMillis();
                    long timeTaken = endTime - startTime;
                    System.out.println("Tid: " + timeTaken + " ms \n");
                    System.out.println("Word not found");
                    System.exit(0);
                }
            }

            // If the hash exists but the word does not exist.
            if (finalLine.length == 0) {
                long endTime = System.currentTimeMillis();
                long timeTaken = endTime - startTime;
                System.out.println("Tid: " + timeTaken + " ms \n");
                System.out.println("Word not found");
                System.exit(0);
            }

            // PRINTING
            // Print number of occurences
            int occurrences = Integer.parseInt(finalLine[2]);
            System.out.println("Det finns " + occurrences + " förekomster av ordet");

            long endTime = System.currentTimeMillis();

            String currentWord = finalLine[0];
            int startIndexPointer = Integer.parseInt(finalLine[1]);

            String lineAfter = wordsFile.readLine();

            // Special case where the next line is null
            if (lineAfter == null) {
                int numberOfResults = Math.min(25, occurrences);
                byte[] byteArray = new byte[30 + currentWord.length() + 30];
                int totalcounter = 0;

                indexFile.seek(startIndexPointer);
                long currIndex = 0;

                while (totalcounter < numberOfResults) {
                    currIndex = Long.parseLong(indexFile.readLine());
                    if ((currIndex - 30) < 0) {
                        korpusFile.seek(0);
                    } else {
                        korpusFile.seek(currIndex - 30);

                    }

                    int bytesRead = korpusFile.read(byteArray);
                    if (bytesRead == -1) {
                        break; // Exit loop if end of file is reached
                    }
                    String sentence = new String(byteArray, 0, bytesRead, "ISO-8859-1").replace("\n", " ");
                    totalcounter++;
                    System.out.println(totalcounter + "\t" + sentence);
                }
            }

            // If next line exists
            indexFile.seek(startIndexPointer);
            String sentence = "";
            long currIndex = 0;

            byte[] byteArray = new byte[30 + currentWord.length() + 30];

            // First 25 results
            int numberOfResults = 25;
            if (occurrences < 25)
                numberOfResults = occurrences;

            for (int in = 1; in <= numberOfResults; in++) {
                currIndex = Long.parseLong(indexFile.readLine());
                if ((currIndex - 30) < 0) {
                    korpusFile.seek(0);
                } else {
                    korpusFile.seek(currIndex - 30);

                }
                korpusFile.read(byteArray);
                sentence = new String(byteArray, "ISO-8859-1").replace("\n", " ");
                System.out.println(in + "\t" + sentence);
            }
            // Print time taken
            long timeTaken = endTime - startTime;
            System.out.println("Tid: " + timeTaken + " ms \n");
            System.out.println(occurrences);
            if (occurrences > 25) {
                int totalcounter = numberOfResults;
                Scanner scan = new Scanner(System.in);

                while (true) {
                    if (totalcounter >= occurrences) {
                        break;
                    }
                    System.out.println("Visa fler? y/n: ");

                    String response = scan.next().toLowerCase();

                    if (response.equals("y")) {
                        for (int i = numberOfResults + 1; i <= occurrences; i++) {
                            currIndex = Long.parseLong(indexFile.readLine());
                            if ((currIndex - 30) < 0) {
                                korpusFile.seek(0);
                            } else {
                                korpusFile.seek(currIndex - 30);

                            }
                            korpusFile.read(byteArray);
                            sentence = new String(byteArray, "ISO-8859-1").replace("\n", " ");
                            System.out.println(i + "\t" + sentence);
                            totalcounter++;
                        }
                    } else if (response.equals("n")) {
                        break;
                    }
                }
            }

        } catch (

        Exception e) {

        }

    }

    /*
     * The function that is used to calculate a hash value
     * 
     */
    private long hashFunction(char[] input) throws Exception {
        long value = 0;
        switch (input.length) {
            case 0:
                break;
            case 1:
                value = convertCharToValue(input[0]) * 900;
                break;
            case 2:
                value = convertCharToValue(input[0]) * 900 + convertCharToValue(input[1]) * 30;
                break;
            default:
                value = convertCharToValue(input[0]) * 900 + convertCharToValue(input[1]) * 30
                        + convertCharToValue(input[2]);
                break;
        }
        return value * 8;

    }

    /*
     * Converts a character to its ASCII value
     */
    private int convertCharToValue(char input) {

        int value = input - 96;

        if (value >= 1 && value <= 26) {
            return value;
        } else if (value == 132) { // ä
            return 27;
        } else if (value == 133) { // å
            return 28;
        } else if (value == 150) { // ö
            return 29;
        }
        return 0;

    }

    /*
     * Constructs the hash file
     * 
     */
    private void constructHash() {

        try {
            BufferedReader wordFiles = new BufferedReader(
                    new InputStreamReader(new FileInputStream(words), StandardCharsets.ISO_8859_1)); // The file
                                                                                                     // containing the
                                                                                                     // words

            RandomAccessFile binaryFile = new RandomAccessFile(hash, "rw"); // The hash file

            binaryFile.setLength((900 * 30 + 1) * 8); // Initializes the size of the binary hash file to 8 times the
                                                      // number of possible hashes.

            String nextLine = "";
            String prefix = "";
            String previousPrefix = "";

            long hashIndex = 0;
            long wordsByteCount = 0;

            // Go through each word and hash it
            while ((nextLine = wordFiles.readLine()) != null) {
                String[] splitLine = nextLine.trim().split(" ");

                if (splitLine[0].length() > 3) {
                    prefix = splitLine[0].substring(0, 3); // Takes first three letters of word if word is longer

                } else {
                    prefix = splitLine[0];
                }
                if (!prefix.equals(previousPrefix)) {
                    // Calculate hashValue
                    hashIndex = hashFunction(prefix.toCharArray());
                    binaryFile.seek(hashIndex);
                    // Seek to the hashIndex in the binary file and write the index.
                    binaryFile.writeLong(wordsByteCount);

                    previousPrefix = prefix;

                }
                wordsByteCount += (nextLine + "\n").length();

            }
            System.out.println("Done with hash");
            binaryFile.close();
            wordFiles.close();
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    private void constructConcordance() {
        BufferedInputStream rawIndex = Mio.OpenRead("rawindex.txt"); // Read raw index file
        System.out.println("Creating word and index files");

        try {
            words.createNewFile(); // Tries to create new files, fails otherwise.
            indexes.createNewFile();
            hash.createNewFile();
            System.out.println("Files created");
        } catch (Exception e) {
            System.out.println("Failed to create files");
        }

        try {
            Writer outWords = new BufferedWriter(
                    new OutputStreamWriter(new FileOutputStream(words), StandardCharsets.ISO_8859_1));
            Writer outIndex = new BufferedWriter(
                    new OutputStreamWriter(new FileOutputStream(indexes), StandardCharsets.ISO_8859_1));

            long indexByteCounter = 0; // Gives position in text file
            long prevByteCount = 0;

            int index;

            String previousWord = Mio.GetWord(rawIndex).toLowerCase();
            String word = "";

            int wordCount = 0;

            while (!Mio.EOF(rawIndex)) { // We iterate through the rawIndex fil, one line at a time.

                index = Mio.GetInt(rawIndex); // We get the index stored in the current position of the raw index file.
                indexByteCounter += (index + "\n").length(); // We then create the byte pointer to the position in the
                                                             // file
                wordCount++; // Increase the number of words we have seen.
                outIndex.append(index + "\n"); // Writes the index of the word to the index file.
                word = Mio.GetWord(rawIndex).toLowerCase();

                if (!previousWord.equals(word)) { // only append unique words to the wordsfile

                    // Concatanates previousWord, prevByteCount as well as the number of words to be
                    // written to the words file and writes it.
                    String output = String.format(previousWord + " " + prevByteCount + " " + wordCount + "\n");

                    outWords.append(output);

                    wordCount = 0;
                    prevByteCount = indexByteCounter;
                    previousWord = word;
                }

            }

            outWords.close();
            outIndex.close();
        } catch (Exception e) {

        }

    }

}