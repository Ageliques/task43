#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_WORK_LENGTH 100
#define MAX_POSITION_LENGTH 50
#define MAX_PHONE_LENGTH 20
#define MAX_EMAIL_LENGTH 100
#define MAX_SOCIAL_LENGTH 100

typedef struct Contact {
    char first_name[MAX_NAME_LENGTH];
    char surname[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char work[MAX_WORK_LENGTH];
    char position[MAX_POSITION_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char social[MAX_SOCIAL_LENGTH];
    int height; // Высота узла
    struct Contact *left;
    struct Contact *right;
} Contact;

Contact *root = NULL;

// Функция для создания нового контакта
Contact* create_contact(char *first_name, char *surname, char *last_name, 
                        char *work, char *position, char *phone, 
                        char *email, char *social) {
    Contact *new_contact = (Contact*)malloc(sizeof(Contact));
    strcpy(new_contact->first_name, first_name);
    strcpy(new_contact->surname, surname);
    strcpy(new_contact->last_name, last_name);
    strcpy(new_contact->work, work);
    strcpy(new_contact->position, position);
    strcpy(new_contact->phone, phone);
    strcpy(new_contact->email, email);
    strcpy(new_contact->social, social);
    new_contact->left = new_contact->right = NULL;
    new_contact->height = 1; // Высота нового узла равна 1
    return new_contact;
}

// Функция для получения высоты узла
int height(Contact *node) {
    return node ? node->height : 0;
}

// Функция для получения баланса узла
int get_balance(Contact *node) {
    if (node == NULL) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

// Правый поворот
Contact* right_rotate(Contact *y) {
    Contact *x = y->left;
    Contact *T2 = x->right;

    // Выполняем поворот
    x->right = y;
    y->left = T2;

    // Обновляем высоты
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));

    return x; // Возвращаем новый корень
}

// Левый поворот
Contact* left_rotate(Contact *x) {
    Contact *y = x->right;
    Contact *T2 = y->left;

    // Выполняем поворот
    y->left = x;
    x->right = T2;

    // Обновляем высоты
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));

    return y; // Возвращаем новый корень
}

// Вставка контакта в AVL-дерево
Contact* insert(Contact *node, Contact *new_contact) {
    if (node == NULL) {
        return new_contact;
    }

    if (strcmp(new_contact->surname, node->surname) < 0) {
        node->left = insert(node->left, new_contact);
    } else if (strcmp(new_contact->surname, node->surname) > 0) {
        node->right = insert(node->right, new_contact);
    } else {
        printf("Contact with surname %s already exists.\n", new_contact->surname);
        return node; // Дубликаты не добавляем
    }

    // Обновляем высоту текущего узла
    node->height = 1 + (height(node->left) > height(node->right) ? height(node->left) : height(node->right));

    // Получаем баланс
    int balance = get_balance(node);

    // Балансировка дерева

    // Левый левый случай
    if (balance > 1 && strcmp(new_contact->surname, node->left->surname) < 0) {
        return right_rotate(node);
    }

    // Правый правый случай
    if (balance < -1 && strcmp(new_contact->surname, node->right->surname) > 0) {
        return left_rotate(node);
    }

    // Левый правый случай
    if (balance > 1 && strcmp(new_contact->surname, node->left->surname) > 0) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    // Правый левый случай
    if (balance < -1 && strcmp(new_contact->surname, node->right->surname) < 0) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node; // Возвращаем неизменённый узел
}

// Печать всех контактов в порядке возрастания
void inorder_print(Contact *node) {
    if (node != NULL) {
        inorder_print(node->left);
        printf("Name: %s %s %s\n", node->first_name, node->surname, node->last_name);
        printf("Work: %s\n", node->work);
        printf("Position: %s\n", node->position);
        printf("Phone: %s\n", node->phone);
        printf("Email: %s\n", node->email);
        printf("Social: %s\n", node->social);
        printf("------------------------\n");
        inorder_print(node->right);
    }
}

// Функция для поиска контакта по имени и фамилии
Contact* find_contact(Contact *node, char *first_name, char *surname) {
    if (node == NULL) {
        return NULL;
    }

    if (strcmp(surname, node->surname) == 0 && strcmp(first_name, node->first_name) == 0) {
        return node; // Найден контакт
    }

    if (strcmp(surname, node->surname) < 0) {
        return find_contact(node->left, first_name, surname);
    }

    return find_contact(node->right, first_name, surname);
}

// Функция для редактирования контакта
void edit_contact(Contact *node) {
    char first_name[MAX_NAME_LENGTH], surname[MAX_NAME_LENGTH];

    printf("Enter first name of the contact to edit: ");
    scanf("%s", first_name);
    
    printf("Enter surname of the contact to edit: ");
    scanf("%s", surname);

    Contact *contact_to_edit = find_contact(node, first_name, surname);

    if (contact_to_edit != NULL) {
        printf("Editing contact:\n");
        printf("Current details:\n");
        printf("First Name: %s\n", contact_to_edit->first_name);
        printf("Surname: %s\n", contact_to_edit->surname);
        printf("Last Name: %s\n", contact_to_edit->last_name);
        printf("Work: %s\n", contact_to_edit->work);
        printf("Position: %s\n", contact_to_edit->position);
        printf("Phone: %s\n", contact_to_edit->phone);
        printf("Email: %s\n", contact_to_edit->email);
        printf("Social: %s\n", contact_to_edit->social);

        // Запрашиваем новые данные у пользователя
        printf("Enter new last name (or press enter to keep current): ");
        char new_last_name[MAX_NAME_LENGTH];
        getchar(); // Очищаем буфер перед вводом строки
        fgets(new_last_name, sizeof(new_last_name), stdin); 

        if (strlen(new_last_name) > 1) { // Если введена новая фамилия
            new_last_name[strcspn(new_last_name, "\n")] = '0'; // Убираем символ новой строки
            strcpy(contact_to_edit->last_name, new_last_name);
        }

        printf("Enter new work (or press enter to keep current): ");
        char new_work[MAX_WORK_LENGTH];
        fgets(new_work, sizeof(new_work), stdin); 

        if (strlen(new_work) > 1) {
            new_work[strcspn(new_work, "\n")] = '0';
            strcpy(contact_to_edit->work, new_work);
        }

        printf("Enter new position (or press enter to keep current): ");
        char new_position[MAX_POSITION_LENGTH];
        fgets(new_position, sizeof(new_position), stdin); 

        if (strlen(new_position) > 1) {
            new_position[strcspn(new_position, "\n")] = '0';
            strcpy(contact_to_edit->position, new_position);
        }

        printf("Enter new phone (or press enter to keep current): ");
        char new_phone[MAX_PHONE_LENGTH];
        fgets(new_phone, sizeof(new_phone), stdin); 

        if (strlen(new_phone) > 1) {
            new_phone[strcspn(new_phone, "\n")] = '0';
            strcpy(contact_to_edit->phone, new_phone);
        }

        printf("Enter new email (or press enter to keep current): ");
        char new_email[MAX_EMAIL_LENGTH];
        fgets(new_email, sizeof(new_email), stdin); 

        if (strlen(new_email) > 1) {
            new_email[strcspn(new_email, "\n")] = '0';
            strcpy(contact_to_edit->email, new_email);
        }

        printf("Enter new social media (or press enter to keep current): ");
        char new_social[MAX_SOCIAL_LENGTH];
        fgets(new_social, sizeof(new_social), stdin); 

        if (strlen(new_social) > 1) {
            new_social[strcspn(new_social, "\n")] = '0';
            strcpy(contact_to_edit->social, new_social);
        }

        printf("Contact updated successfully.\n");
        
    } else {
        printf("Contact not found.\n");
    }
}

// Функция для нахождения узла с минимальным значением
Contact* min_value_node(Contact* node) {
    Contact* current = node;

    while (current && current->left != NULL)
        current = current->left;

    return current;
}

// Функция для удаления контакта из AVL-дерева
Contact* delete_contact(Contact* root, char* first_name, char* surname) {
    if (root == NULL)
        return root;

    if (strcmp(surname, root->surname) < 0)
        root->left = delete_contact(root->left, first_name, surname);
    
    else if (strcmp(surname, root->surname) > 0)
        root->right = delete_contact(root->right, first_name, surname);

    else { // Найден узел для удаления
        if (strcmp(first_name, root->first_name) == 0) { // Удаляем только если имя совпадает

            // Узел с одним ребенком или без детей
            if ((root->left == NULL) || (root->right == NULL)) {
                Contact* temp = root->left ? root->left : root->right;

                // Если нет детей
                if (temp == NULL) {
                    temp = root;
                    root = NULL; // Узел удален
                } else // Узел с одним ребенком
                    *root = *temp; // Копируем содержимое дочернего узла

                free(temp); // Освобождаем память
            } else {
                // Узел с двумя детьми: получаем минимальный узел из правого поддерева
                Contact* temp = min_value_node(root->right);

                // Копируем данные минимального узла в текущий узел
                strcpy(root->first_name, temp->first_name);
                strcpy(root->surname, temp->surname);
                strcpy(root->last_name, temp->last_name);
                strcpy(root->work, temp->work);
                strcpy(root->position, temp->position);
                strcpy(root->phone, temp->phone);
                strcpy(root->email, temp->email);
                strcpy(root->social, temp->social);

                // Удаляем минимальный узел из правого поддерева
                root->right = delete_contact(root->right, temp->first_name, temp->surname);
            }
        }
    }

    // Если дерево имело только один узел
    if (root == NULL)
      return root;

   // Обновляем высоту текущего узла
   root->height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));

   // Получаем баланс
   int balance = get_balance(root);

   // Балансировка дерева

   // Левый левый случай
   if (balance > 1 && get_balance(root->left) >= 0)
       return right_rotate(root);

   // Левый правый случай
   if (balance > 1 && get_balance(root->left) < 0) {
       root->left = left_rotate(root->left);
       return right_rotate(root);
   }

   // Правый правый случай
   if (balance < -1 && get_balance(root->right) <= 0)
       return left_rotate(root);

   // Правый левый случай
   if (balance < -1 && get_balance(root->right) > 0) {
       root->right = right_rotate(root->right);
       return left_rotate(root);
   }

   return root; // Возвращаем неизменённый узел
}

// Основное меню
void menu() {
    int choice;

    while (1) {
        printf("\nPhone Book Menu:\n");
        printf("1. Add Contact\n");
        printf("2. Edit Contact\n");
        printf("3. Delete Contact\n");
        printf("4. List Contacts\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        
        // Очищаем буфер ввода после scanf()
        while(getchar() != '\n');

        switch (choice) {
            case 1: {
                char first_name[MAX_NAME_LENGTH], surname[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char work[MAX_WORK_LENGTH], position[MAX_POSITION_LENGTH];
                char phone[MAX_PHONE_LENGTH], email[MAX_EMAIL_LENGTH], social[MAX_SOCIAL_LENGTH];

                printf("Enter first name: ");
                fgets(first_name, sizeof(first_name), stdin);
                first_name[strcspn(first_name, "\n")] = '0'; // Убираем символ новой строки

                printf("Enter surname: ");
                fgets(surname, sizeof(surname), stdin);
                surname[strcspn(surname, "\n")] = '0';

                printf("Enter last name: ");
                fgets(last_name, sizeof(last_name), stdin);
                last_name[strcspn(last_name, "\n")] = '0';

                printf("Enter work: ");
                fgets(work, sizeof(work), stdin);
                work[strcspn(work, "\n")] = '0';

                printf("Enter position: ");
                fgets(position, sizeof(position), stdin);
                position[strcspn(position, "\n")] = '0';

                printf("Enter phone: ");
                fgets(phone, sizeof(phone), stdin);
                phone[strcspn(phone, "\n")] = '0';

                printf("Enter email: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = '0';

                printf("Enter social media: ");
                fgets(social, sizeof(social), stdin);
                social[strcspn(social, "\n")] = '0';

                Contact *new_contact = create_contact(first_name, surname, last_name,
                                                      work, position, phone,
                                                      email, social);
                root = insert(root, new_contact);
                break;
            }
            case 2:
                edit_contact(root); // Заменено на edit_contact
                break;
            case 3: {
                char first_name[MAX_NAME_LENGTH], surname[MAX_NAME_LENGTH];
                
                printf("Enter first name of the contact to delete: ");
                scanf("%s", first_name);

                printf("Enter surname of the contact to delete: ");
                scanf("%s", surname);

                root = delete_contact(root, first_name, surname); 
                
                break;
            }
            case 4:
                inorder_print(root);
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

int main() {
    menu();
    return 0;
}
