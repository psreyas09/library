#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100
#define FILENAME "library.dat"

typedef struct {
    int bookID; // Unique ID for the book
    char title[100]; // Required book name
    char author[100];
    int totalQuantity; // Total copies of the book
    int availableQuantity; // Copies available for borrowing
} Book;

Book library[MAX_BOOKS];
int bookCount = 0;

void loadBooks() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) return;

    bookCount = fread(library, sizeof(Book), MAX_BOOKS, file);
    fclose(file);
}

void saveBooks() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Error saving books!\n");
        return;
    }

    fwrite(library, sizeof(Book), bookCount, file);
    fclose(file);
}

void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Library is full!\n");
        return;
    }

    Book newBook;

    // Input book ID
    printf("Enter book ID: ");
    scanf("%d", &newBook.bookID);

    // Check if the book ID already exists
    for (int i = 0; i < bookCount; i++) {
        if (library[i].bookID == newBook.bookID) {
            printf("Book with this ID already exists!\n");
            return;
        }
    }

    // Input book title (required)
    printf("Enter book title: ");
    getchar(); // Clear buffer
    fgets(newBook.title, sizeof(newBook.title), stdin);
    newBook.title[strcspn(newBook.title, "\n")] = '\0'; // Remove newline

    // Input book author
    printf("Enter book author: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    newBook.author[strcspn(newBook.author, "\n")] = '\0'; // Remove newline

    // Input book quantity
    printf("Enter total quantity of the book: ");
    scanf("%d", &newBook.totalQuantity);

    // Set available quantity to total quantity initially
    newBook.availableQuantity = newBook.totalQuantity;

    // Add the book to the library
    library[bookCount++] = newBook;
    printf("Book added successfully!\n");
}

void displayBooks() {
    if (bookCount == 0) {
        printf("No books available!\n");
        return;
    }

    printf("\nLibrary Books:\n");
    printf("%-10s %-20s %-20s %-10s %-10s\n", "ID", "Title", "Author", "Total", "Available"); // Header
    for (int i = 0; i < bookCount; i++) {
        printf("%-10d %-20s %-20s %-10d %-10d\n", library[i].bookID, library[i].title, library[i].author, library[i].totalQuantity, library[i].availableQuantity);
    }
}

void searchBook() {
    int id;
    printf("Enter book ID to search: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input! Please enter a valid book ID.\n");
        while (getchar() != '\n'); // Clear the input buffer
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        if (library[i].bookID == id) {
            printf("\nBook Found:\n");
            printf("ID: %d\nTitle: %s\nAuthor: %s\nTotal Quantity: %d\nAvailable Quantity: %d\n", library[i].bookID, library[i].title, library[i].author, library[i].totalQuantity, library[i].availableQuantity);
            return;
        }
    }

    printf("Book not found!\n");
}

void removeBook() {
    int id;
    printf("Enter book ID to remove: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input! Please enter a valid book ID.\n");
        while (getchar() != '\n'); // Clear the input buffer
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        if (library[i].bookID == id) {
            // Shift all books after the removed book one position to the left
            for (int j = i; j < bookCount - 1; j++) {
                library[j] = library[j + 1];
            }
            bookCount--; // Decrease the book count
            printf("Book removed successfully!\n");
            return;
        }
    }

    printf("Book not found!\n");
}

void borrowBook() {
    int id;
    printf("Enter book ID to borrow: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input! Please enter a valid book ID.\n");
        while (getchar() != '\n'); // Clear the input buffer
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        if (library[i].bookID == id) {
            if (library[i].availableQuantity > 0) {
                library[i].availableQuantity--;
                printf("Book borrowed successfully!\n");
                printf("Remaining available copies: %d\n", library[i].availableQuantity);
            } else {
                printf("No available copies of this book to borrow!\n");
            }
            return;
        }
    }

    printf("Book not found!\n");
}

void returnBook() {
    int id;
    printf("Enter book ID to return: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input! Please enter a valid book ID.\n");
        while (getchar() != '\n'); // Clear the input buffer
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        if (library[i].bookID == id) {
            if (library[i].availableQuantity < library[i].totalQuantity) {
                library[i].availableQuantity++;
                printf("Book returned successfully!\n");
                printf("Remaining available copies: %d\n", library[i].availableQuantity);
            } else {
                printf("All copies of this book are already available!\n");
            }
            return;
        }
    }

    printf("Book not found!\n");
}

int main() {
    int choice;

    // Load books from file at startup
    loadBooks();

    do {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book by ID\n");
        printf("4. Remove Book\n");
        printf("5. Borrow Book\n");
        printf("6. Return Book\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number between 1 and 7.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                displayBooks();
                break;
            case 3:
                searchBook();
                break;
            case 4:
                removeBook();
                break;
            case 5:
                borrowBook();
                break;
            case 6:
                returnBook();
                break;
            case 7:
                saveBooks(); // Save data before exiting
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}
