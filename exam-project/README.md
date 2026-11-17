# Design and Implementation of a Multithreaded Web Content Processing System Using FPGA-Accelerated MapReduce

## Concept

The user supplies a URL, and the application asynchronously fetches the webpage, extracts the text, and processes it to count the frequency of each word. The processing is accelerated using an FPGA that implements a MapReduce algorithm for word counting:

1. Fetches the webpage over HTTP/HTTPS.
2. Extracts the textual content from the HTML.
3. Normalizes the text (lowercase, punctuation removal, etc.).
4. Divides the text into chunks.
5. Sends the chunks to an FPGA accelerator.
6. The FPGA performs the MapReduce word-count operation.
7. The software collects the partial results.
8. The software combines the results into a final word-frequency table.
9. The application displays the most frequently used words and their counts.
10. The application saves the results to a txt-file for further analysis.

### Concept example

For example, imagine supplying a link to an article containing:

The cat sat on the mat. The cat was happy.

The final result would be: most used word: "the" (3)
