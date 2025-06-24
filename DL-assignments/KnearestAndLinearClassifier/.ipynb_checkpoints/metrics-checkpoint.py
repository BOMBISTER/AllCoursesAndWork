import numpy as np
def binary_classification_metrics(prediction, ground_truth):
    
    # Преобразуем предсказания и истинные метки в целые числа для подсчета
    TP = np.sum((prediction == 1) & (ground_truth == 1))  # Истинно положительные
    TN = np.sum((prediction == 0) & (ground_truth == 0))  # Истинно отрицательные
    FP = np.sum((prediction == 1) & (ground_truth == 0))  # Ложно положительные
    FN = np.sum((prediction == 0) & (ground_truth == 1))  # Ложно отрицательные

    # Precision (точность) = TP / (TP + FP)
    precision = TP / (TP + FP) if (TP + FP) > 0 else 0

    # Recall (полнота) = TP / (TP + FN)
    recall = TP / (TP + FN) if (TP + FN) > 0 else 0

    # F1-score = 2 * (Precision * Recall) / (Precision + Recall)
    f1 = 2 * (precision * recall) / (precision + recall) if (precision + recall) > 0 else 0

    # Accuracy (точность) = (TP + TN) / (TP + TN + FP + FN)
    accuracy = (TP + TN) / (TP + TN + FP + FN) if (TP + TN + FP + FN) > 0 else 0

    return precision, recall, f1, accuracy



def multiclass_accuracy(prediction, ground_truth):
    '''
    Computes metrics for multiclass classification

    Arguments:
    prediction, np array of int (num_samples) - model predictions
    ground_truth, np array of int (num_samples) - true labels

    Returns:
    accuracy - ratio of accurate predictions to total samples
    '''
    result_good = 0
    size = len(ground_truth)

    for i in range(size):
        if prediction[i] == ground_truth[i]:
            result_good += 1
    
    return result_good/size
