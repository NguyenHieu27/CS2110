;; ============================================================================
;; CS 2110 - Fall 2023
;; Project 3 - wordProcessor
;; ============================================================================
;; Name:
;; ============================================================================

;; =============================== absoluteSum ===============================
;; DESCRIPTION:
;; This function calculates the sum of the absolute values of an array of 
;; integers.
;; The starting address of the array and the length of the array are provided
;; in memory The answer should also be stored in memory at x3050 (ANSWER)

;; SUGGESTED PSUEDOCODE:
;; answer = 0
;; currNum = 0
;; i = 0
;; arrLength = ARR.length()
;; while (arrLength > 0)
;;    currNum = ARR[i]
;;    if (currNum < 0):
;;        currNum = -(currNum); 
;;    answer += currNum;
;;    i++
;;    arrLength--
;; return

.orig x3000
;; YOUR CODE HERE!
    AND R0, R0, 0           ;; answer = 0
    AND R1, R1, 0           ;; currNum = 0
    AND R2, R2, 0           ;; i = 0
    
    LD R3, LEN              ;; arrLength = ARR.length;
    BRnz END_ALL            ;; if arrLength <= 0, break immediately and return
    
WHILE   AND R1, R1, 0       ;; reset currNum
        LD R4, ARR          ;; load array address to R4
        ADD R4, R4, R2      ;; redirect R4 pointer to ARR[i]
        LDR R1, R4, 0       ;; currNum = ARR[i]
        BRzp IS_POSITIVE    ;; if (currNum < 0):
        NOT R1, R1          ;; currNum = -(currNum);
        ADD R1, R1, 1
    
IS_POSITIVE                 ;; skip the if statement
        ADD R0, R0, R1      ;; answer += currNum;
        
        ADD R2, R2, 1       ;; i++;
        ADD R3, R3, -1      ;; arrLength--;
        BRp WHILE           ;; while (arrLength > 0)
        
END_ALL 
        LD R5, ANSWER       
        STR R0, R5, 0       ;; return
        
        HALT

;; Do not rename or remove any existing labels
LEN      .fill 5
ARR      .fill x6000
ANSWER   .fill x3050
.end

;; Answer needs to be stored here
.orig x3050
.blkw 1
.end

;; Array. Change values here for debugging!
.orig x6000
    .fill -3
    .fill 4
    .fill -1
    .fill 10
    .fill -20
.end