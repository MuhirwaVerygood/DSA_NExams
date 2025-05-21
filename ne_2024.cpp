#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure definitions
struct Patient {
    int patient_id;
    char name[50];
    char dob[11]; // Format: YYYY-MM-DD
    char gender[10];
    struct Patient* next;
};

struct Doctor {
    int doctor_id;
    char name[50];
    char specialization[50];
    struct Doctor* next;
};

struct Appointment {
    int appointment_id;
    int patient_id;
    int doctor_id;
    char appointment_date[11]; // Format: YYYY-MM-DD
    struct Appointment* next;
};

// Global pointers to linked lists
struct Patient* patients = NULL;
struct Doctor* doctors = NULL;
struct Appointment* appointments = NULL;

// Function prototypes
void registerPatient();
void registerDoctor();
void registerAppointment();
void displayPatients();
void displayDoctors();
void displayAppointments();
struct Patient* findPatient(int patient_id);
struct Doctor* findDoctor(int doctor_id);
struct Appointment* findAppointment(int appointment_id);
void freeLists();

// Function to register a patient
void registerPatient() {
    struct Patient* newPatient = (struct Patient*)malloc(sizeof(struct Patient));
    printf("Enter patient ID: ");
    scanf("%d", &newPatient->patient_id);
    
    // Check if patient ID exists
    if (findPatient(newPatient->patient_id) != NULL) {
        printf("Error: Patient ID %d already exists!\n", newPatient->patient_id);
        free(newPatient);
        return;
    }
    
    printf("Enter patient name: ");
    scanf(" %[^\n]", newPatient->name);
    printf("Enter date of birth (YYYY-MM-DD): ");
    scanf("%s", newPatient->dob);
    printf("Enter gender: ");
    scanf("%s", newPatient->gender);
    
    newPatient->next = patients;
    patients = newPatient;
    printf("Patient registered successfully!\n");
}

// Function to register a doctor
void registerDoctor() {
    struct Doctor* newDoctor = (struct Doctor*)malloc(sizeof(struct Doctor));
    printf("Enter doctor ID: ");
    scanf("%d", &newDoctor->doctor_id);
    
    // Check if doctor ID exists
    if (findDoctor(newDoctor->doctor_id) != NULL) {
        printf("Error: Doctor ID %d already exists!\n", newDoctor->doctor_id);
        free(newDoctor);
        return;
    }
    
    printf("Enter doctor name: ");
    scanf(" %[^\n]", newDoctor->name);
    printf("Enter specialization: ");
    scanf(" %[^\n]", newDoctor->specialization);
    
    newDoctor->next = doctors;
    doctors = newDoctor;
    printf("Doctor registered successfully!\n");
}

// Function to register an appointment
void registerAppointment() {
    struct Appointment* newAppointment = (struct Appointment*)malloc(sizeof(struct Appointment));
    printf("Enter appointment ID: ");
    scanf("%d", &newAppointment->appointment_id);
    
    // Check if appointment ID exists
    if (findAppointment(newAppointment->appointment_id) != NULL) {
        printf("Error: Appointment ID %d already exists!\n", newAppointment->appointment_id);
        free(newAppointment);
        return;
    }
    
    printf("Enter patient ID: ");
    scanf("%d", &newAppointment->patient_id);
    printf("Enter doctor ID: ");
    scanf("%d", &newAppointment->doctor_id);
    
    // Check if patient and doctor exist
    if (findPatient(newAppointment->patient_id) == NULL) {
        printf("Error: Patient ID %d does not exist!\n", newAppointment->patient_id);
        free(newAppointment);
        return;
    }
    if (findDoctor(newAppointment->doctor_id) == NULL) {
        printf("Error: Doctor ID %d does not exist!\n", newAppointment->doctor_id);
        free(newAppointment);
        return;
    }
    
    printf("Enter appointment date (YYYY-MM-DD): ");
    scanf("%s", newAppointment->appointment_date);
    
    newAppointment->next = appointments;
    appointments = newAppointment;
    printf("Appointment registered successfully!\n");
}

// Function to find a patient by ID
struct Patient* findPatient(int patient_id) {
    struct Patient* current = patients;
    while (current != NULL) {
        if (current->patient_id == patient_id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to find a doctor by ID
struct Doctor* findDoctor(int doctor_id) {
    struct Doctor* current = doctors;
    while (current != NULL) {
        if (current->doctor_id == doctor_id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to find an appointment by ID
struct Appointment* findAppointment(int appointment_id) {
    struct Appointment* current = appointments;
    while (current != NULL) {
        if (current->appointment_id == appointment_id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to display all patients
void displayPatients() {
    struct Patient* current = patients;
    if (current == NULL) {
        printf("No patients registered.\n");
        return;
    }
    printf("\nPatients List:\n");
    printf("ID\tName\t\tDOB\t\tGender\n");
    printf("------------------------------------------------\n");
    while (current != NULL) {
        printf("%d\t%s\t%s\t%s\n", current->patient_id, current->name, current->dob, current->gender);
        current = current->next;
    }
}

// Function to display all doctors
void displayDoctors() {
    struct Doctor* current = doctors;
    if (current == NULL) {
        printf("No doctors registered.\n");
        return;
    }
    printf("\nDoctors List:\n");
    printf("ID\tName\t\tSpecialization\n");
    printf("------------------------------------------------\n");
    while (current != NULL) {
        printf("%d\t%s\t%s\n", current->doctor_id, current->name, current->specialization);
        current = current->next;
    }
}

// Function to display all appointments
void displayAppointments() {
    struct Appointment* current = appointments;
    if (current == NULL) {
        printf("No appointments registered.\n");
        return;
    }
    printf("\nAppointments List:\n");
    printf("ID\tPatient ID\tDoctor ID\tDate\n");
    printf("------------------------------------------------\n");
    while (current != NULL) {
        printf("%d\t%d\t\t%d\t\t%s\n", current->appointment_id, current->patient_id, current->doctor_id, current->appointment_date);
        current = current->next;
    }
}

// Function to free all linked lists
void freeLists() {
    struct Patient* patient = patients;
    while (patient != NULL) {
        struct Patient* temp = patient;
        patient = patient->next;
        free(temp);
    }
    
    struct Doctor* doctor = doctors;
    while (doctor != NULL) {
        struct Doctor* temp = doctor;
        doctor = doctor->next;
        free(temp);
    }
    
    struct Appointment* appointment = appointments;
    while (appointment != NULL) {
        struct Appointment* temp = appointment;
        appointment = appointment->next;
        free(temp);
    }
}

// Main function with menu
int main() {
    int choice;
    while (1) {
        printf("\nRuhengeri Referral Hospital Management System\n");
        printf("1. Register Patient\n");
        printf("2. Register Doctor\n");
        printf("3. Register Appointment\n");
        printf("4. Display Patients\n");
        printf("5. Display Doctors\n");
        printf("6. Display Appointments\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                registerPatient();
                break;
            case 2:
                registerDoctor();
                break;
            case 3:
                registerAppointment();
                break;
            case 4:
                displayPatients();
                break;
            case 5:
                displayDoctors();
                break;
            case 6:
                displayAppointments();
                break;
            case 7:
                freeLists();
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}
