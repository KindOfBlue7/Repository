# IMPORT START
import math

from typing import List, Tuple, Set, Dict
import random
import numpy as np
from collections import OrderedDict
import timeit
import io
from contextlib import redirect_stdout
import copy


# IMPORT END


# DEFINES START
solutionType = List[int]

# DEFINES END

class Individual:

    def __init__(self, permutation: List[int]):
        self.sol_vector: List[int] = permutation

    def switch_mutation(self):
        """Performs a two point switch mutation"""
        cxpoint1: int = random.randint(0, len(self.sol_vector) - 1)
        cxpoint2: int = random.randint(0, len(self.sol_vector) - 1)
        # cntr is a fail sale to prevent infinity loop when population is too small
        cntr = 0
        while cxpoint1 == cxpoint2:
            cntr = cntr + 1
            cxpoint2 = random.randint(0, len(self.sol_vector) - 1)
            if cntr == 5:
                print('There is a problem with mutating, probably population size is too small.')
                break
        # print(f'CX points are {cxpoint1} {cxpoint2}')

        self.sol_vector[cxpoint1], self.sol_vector[cxpoint2] = self.sol_vector[cxpoint2], self.sol_vector[cxpoint1]

    def shift_mutation(self):
        """Performs a shifting mutation, random length, moves to start or the end with ~1/2
        probability """

        cxpoint1: int = random.randint(0, len(self.sol_vector) - 1)
        cxpoint2: int = random.randint(0, len(self.sol_vector) - 1)
        cntr: int = 0
        while cxpoint1 == cxpoint2:
            cntr = cntr + 1
            cxpoint2 = random.randint(0, len(self.sol_vector) - 1)
            if cntr == 5:
                print('There is a problem with mutating, probably population size is too small.')
                break
        if cxpoint1 < cxpoint2:
            temp: solutionType = self.sol_vector[cxpoint1:cxpoint2]
            del (self.sol_vector[cxpoint1:cxpoint2])
        else:
            temp: solutionType = self.sol_vector[cxpoint2:cxpoint1]
            del (self.sol_vector[cxpoint2:cxpoint1])

        if random.random() < 1 / 2:
            self.sol_vector = temp + self.sol_vector
            del temp

        else:
            self.sol_vector = self.sol_vector + temp
            del temp


# DEFINE
populationType = Dict[Individual, int]


# DEFINE


class Population:

    def __init__(self, initial_cardinality: int, iterations: int,
                 mutation_prob: float, selection_constant: int,
                 random_matrix: bool = True, ox: bool = True,
                 tasks: int = 9, machines: int = 9, min_val: int = 0,
                 max_val: int = 10, ):
        """

        :param tasks: tasks amount
        :param machines: machines amount
        :param initial_cardinality: population cardinality
        :param iterations: number of iterations for simulation
        :param mutation_prob: mutation probability
        :param selection_constant: constant for tournament selection
        :param random_matrix: True - random problem matrix, False - from file
        :param ox: True - ox crossover, False = pmx crossover
        :param min_val: minimal value for problem matrix generation
        :param max_val: maximal value for problem matrix generation
        """
        # creation of problem matrix:
        
        if random_matrix:
            self.tasks: int = tasks
            self.machines: int = machines
            self.problem_matrix: np.ndarray = np.random.randint(min_val, max_val, (tasks, machines))
        else:
            self.tasks, self.machines, self.problem_matrix = self.read_from_file()
            
        # creation of population collection
        self.population: OrderedDict[Individual, int] = OrderedDict()
        # simulation parameters
        self.cardinality = initial_cardinality  # cardinality of population
        self.selection_constant: int = selection_constant  # constant for tournament selection in cross-breeding
        self.ox: bool = ox #compare results of pmx and ox
        # between individuals
        self.iterations: int = iterations  # iterations amount for simulation
        self.mutation_prob: float = mutation_prob  # probability of mutation
        # simulation results
        self.permutation_cntr: int = 0  # randomly generated permutations
        self.best_solution: Tuple[List[int], int] = ([], 999999999999999999999999)  # starting best solution container
        self.function_history: List[int] = []  # changing values of best_solution
        self.used_permutations: Set[Tuple[int]] = set()  # value of different checked permutations
    
    def read_from_file(self)->List:
        f = open("matrix.txt", "r")
        if f.mode=='r':
            contents = f.read()

        contents = contents.split()
        temp = []
        matrix = []

        for x in contents:
            if x[-1]==';':
                temp.append(int(x[0:-1]))
                matrix.append(temp)
                temp=[]
            else:
                temp.append(int(x))
        f.close()
        
        return [len(matrix),len(matrix[0]),np.array(matrix)]
    
    def target_fcn(self, solution: List[int]):
        solution_helper = np.zeros((self.tasks, self.machines))
        # solution helper - każda komórka to czas w którym skończy się dane zadanie na maszynie/maszyna się zwolni
        for i in range(len(solution)):
            for j in range(len(self.problem_matrix[solution[i]])):
                if solution_helper[i - 1][j] > solution_helper[i][j - 1]:
                    solution_helper[i][j] = solution_helper[i - 1][j] + self.problem_matrix[solution[i] - 1][j]
                else:
                    solution_helper[i][j] = solution_helper[i][j - 1] + self.problem_matrix[solution[i] - 1][j]
        return solution_helper[len(self.problem_matrix) - 1][len(self.problem_matrix[0]) - 1]

    def supplement_population(self):
        """Creates initial population of specified cardinality, or supplements incomplete population
        to defined cardinality"""
        if int(math.factorial(self.tasks)) < self.cardinality:
            print(f'It is illegal, specified value is above maximum amount of possibilities. There are only '
                  f'{int(math.factorial(self.tasks))} possibilities.')
        else:
            current_population: Set[Tuple[int]] = set()

            while len(self.population) < self.cardinality:
                permutation: List[int] = list(np.random.permutation(self.tasks))
                self.population[Individual(permutation)] = self.target_fcn(permutation)
                current_population.add(tuple(permutation))
                self.permutation_cntr += 1

            self.used_permutations = self.used_permutations | current_population
            del current_population

    def demise(self):
        """Randomly kills individuals with 1/2 probability"""
        temp: populationType = OrderedDict()
        for individual, value in self.population.items():
            if random.random() < 1 / 2:
                temp[individual] = value
            else:
                continue
        self.population = temp
        del temp
        self.supplement_population()

    def rank_population(self):
        """Ranks population in a ascending order by the function value"""
        self.population = OrderedDict(sorted(self.population.items(), key=lambda x: x[1]))
        for key, value in self.population.items():
            if value < self.best_solution[1]:
                self.function_history.append(self.best_solution[1])
                self.best_solution = (key.sol_vector, value)

    def print_population(self):
        """Prints population"""
        cntr: int = 0
        for key in self.population:
            cntr = cntr + 1
            print(f'{cntr}) {key.sol_vector} and its value is {self.population[key]}')

    def crossover_pmx(self, breeding_population: populationType) -> None:
        """
        :param breeding_population: population in which breeding will occur
        """
        temp: Set[List[int]] = set()
        # print(len(breeding_population))
        while len(temp) < self.cardinality:
            #print(len(temp))
            ind1 = (random.sample(breeding_population.items(), 1))[0][0].sol_vector
            ind2 = (random.sample(breeding_population.items(), 1))[0][0].sol_vector
            # print(ind2)
            while ind1 == ind2:
                #print('spam')
                ind2 = (random.sample(breeding_population.items(), 1))[0][0].sol_vector
            size = self.tasks
            p1, p2 = [0] * size, [0] * size

            # Initialize the position of each indices in the individuals
            for i in range(size):
                p1[ind1[i]] = i
                p2[ind2[i]] = i
            # Choose crossover points
            cxpoint1 = random.randint(0, size)
            cxpoint2 = random.randint(0, size - 1)
            if cxpoint2 >= cxpoint1:
                cxpoint2 += 1
            else:  # Swap the two cx points
                cxpoint1, cxpoint2 = cxpoint2, cxpoint1

            # Apply crossover between cx points
            for i in range(cxpoint1, cxpoint2):
                # Keep track of the selected values
                temp1 = ind1[i]
                temp2 = ind2[i]
                # Swap the matched value
                ind1[i], ind1[p1[temp2]] = temp2, temp1
                ind2[i], ind2[p2[temp1]] = temp1, temp2
                # Position bookkeeping
                p1[temp1], p1[temp2] = p1[temp2], p1[temp1]
                p2[temp1], p2[temp2] = p2[temp2], p2[temp1]
            temp.add(tuple(ind1))
            temp.add(tuple(ind2))
            self.population = dict()
        for item in temp:
            self.population[Individual(list(item))] = self.target_fcn(list(item))

    def crossover_ox(self, breeding_population: populationType) -> None:
        """
        Two point CX crossover
        :param breeding_population: population in which breeding will occur
        """
        temp: Set[List[int]] = set()
        # print(len(breeding_population))
        while len(temp) < self.cardinality:
            ind1 = (random.sample(breeding_population.items(), 1))[0][0].sol_vector
            ind2 = (random.sample(breeding_population.items(), 1))[0][0].sol_vector
            while ind1 == ind2:
                ind2 = (random.sample(breeding_population.items(), 1))[0][0].sol_vector
            size = self.tasks
            a, b = random.sample(range(size), 2)
            if a > b:
                a, b = b, a

            holes1, holes2 = [True] * size, [True] * size
            for i in range(size):
                if i < a or i > b:
                    holes1[ind2[i]] = False
                    holes2[ind1[i]] = False

            # We must keep the original values somewhere before scrambling everything
            temp1, temp2 = ind1, ind2
            k1, k2 = b + 1, b + 1
            for i in range(size):
                if not holes1[temp1[(i + b + 1) % size]]:
                    ind1[k1 % size] = temp1[(i + b + 1) % size]
                    k1 += 1

                if not holes2[temp2[(i + b + 1) % size]]:
                    ind2[k2 % size] = temp2[(i + b + 1) % size]
                    k2 += 1

            # Swap the content between a and b (included)
            for i in range(a, b + 1):
                ind1[i], ind2[i] = ind2[i], ind1[i]
            temp.add(tuple(ind1))
            temp.add(tuple(ind2))
            self.population = dict()
        for item in temp:
            self.population[Individual(list(item))] = self.target_fcn(list(item))

    def tournament_selection(self):
        """Uses a tournament selection to create a breeding population with size of cardinality/2 which will be bred
        using crossover functions"""
        breeding_population: populationType = OrderedDict()
        while len(breeding_population) < self.cardinality / 2:
            sample_population: populationType = random.sample(self.population.items(), self.selection_constant)
            key, value = min(sample_population, key=lambda x: x[1])
            # print(f'{len(breeding_population)} {key} {value}')
            breeding_population[key] = value

        self.population.clear()

        if self.ox:
            self.crossover_ox(breeding_population)
        else:
            self.crossover_pmx(breeding_population)
        for item in self.population.items():
            self.used_permutations.add(tuple(item[0].sol_vector))

    def simulation(self):
        """Simulation function.
        Simulation schematic is:
            Create initial population
                Cross-breeding
                Mutation
                Repeat for iterations number
            Print results
        """
        self.supplement_population()
        self.rank_population()
        iter_cntr: int = 0
        
        print("Problem Matrix:")
        print(self.problem_matrix)
        print("Crossover operator chosen: ", end="")
        print("OX") if self.ox else print("PMX")
        
        while iter_cntr < self.iterations:
            self.tournament_selection()

            self.supplement_population()

            if random.random() < self.mutation_prob:
                random.sample(self.population.items(), 1)[0][0].switch_mutation()
                # random.sample(self.population.items(), 1)[0][0].shift_mutation()
            self.rank_population()
            iter_cntr += 1
        print(f'Best solution found is: {self.best_solution[0]} with value {self.best_solution[1]}, '
              f'we checked {len(self.used_permutations)} different permutations, and created '
              f'{self.permutation_cntr} random permutations.\n Function value history is {self.function_history}')

def run(pop: Population, compare: bool = False)->None:
    if compare:
        trap = io.StringIO() #to suppres output from simulation func
        pop_ox = copy.copy(pop)
        pop_pmx = copy.deepcopy(pop)
        pop_ox.ox = True
        pop_pmx.ox = False
        
        with redirect_stdout(trap):
            elapsed_time_ox = timeit.timeit(pop_ox.simulation, number=1)
            elapsed_time_pmx = timeit.timeit(pop_pmx.simulation, number=1)    
            
        print("RESULTS COMPARING PMX AND OX OPERATORS:")
        print("Problem Matrix:")
        print(pop_ox.problem_matrix)
        
        print(f'OX Time: {round(elapsed_time_ox,3)}s\n'
              f'PMX Time: {round(elapsed_time_pmx,3)}s\n'
              f'OX: Best solution: {pop_ox.best_solution[0]} with value {pop_ox.best_solution[1]}\n'
              f'PMX: Best solution: {pop_pmx.best_solution[0]} with value {pop_pmx.best_solution[1]}\n')
    else:
        elapsed_time = timeit.timeit(pop.simulation, number=1)
        print(f'Elapsed time: {round(elapsed_time,3)}s')
        
def user_input_int()->int:
    try:
        user_choice: int = int(input())
    except ValueError:
        print("!!!! WRONG CHARACTER INSERTED !!!!")
        return 0
    return user_choice

def param_change(initial_cardinality: int, iterations: int, mutation_prob: float,
                         selection_constant: int, random_matrix: bool, ox: bool,
                         tasks: int, machines: int, min_val: float, max_val:float,
                         compare: bool):
    while 1:
        print("\n------------------------------------")
        print("Which parameter to change?")
        print(f'1. Initial cardinality ({initial_cardinality})\n'
              f'2. Number of iterations ({iterations})\n'
              f'3. Mutation probability ({mutation_prob})\n'
              f'4. Constant for tournament selection ({selection_constant})\n'
              f'5. Crossover operator (', end="")
        if ox:
            print("ox)")
        else:
            print("pmx)")
        print(f'6. Comparing crossover operators ({compare})\n'
              f'7. Problem matrix source (', end="")
        if random_matrix:
            print("Random)")
        else:
            print("matrix.txt)")
        
        print("8. Exit")
            
        print("Your choice: ", end="")
        param_choice: int = user_input_int()
        
        if param_choice == 8:
            return initial_cardinality, iterations, mutation_prob, selection_constant, random_matrix, ox, tasks, machines, min_val, max_val, compare
        
        print("Changing", end="")
        
        if param_choice == 1:
            print("initial cardinality:")
            print("---Insert a number: ", end="")
            initial_cardinality = user_input_int()
            
        elif param_choice == 2:
            print("number of iterations:")
            print("---Insert a number: ", end="")
            iterations = user_input_int()
            
        elif param_choice == 3:
            print("mutation probability:")
            print("---Insert a number: ", end="")
            try:
                mutation_prob = float(input())
            except ValueError:
                print("!!!! WRONG CHARACTER INSERTED !!!!")
            
        elif param_choice == 4:
            print("constant for tournament selection:")
            print("---Insert a number: ", end="")
            selection_constant = user_input_int()
            
        elif param_choice == 5:
            print("crossover operator:")
            print("(1 - ox, 2 - pmx)")
            print("---Insert a number: ", end="")
            temp: int = user_input_int()
            
            if temp == 1:
                ox = True
            elif temp == 2:
                ox = False
            else:
                pass
            
        elif param_choice == 6:
            print("comparison of operators:\n(True or False)")
            print("---Type your choice: ", end="")
            try:
                compare = bool(input())
            except ValueError:
                print("!!!! WRONG CHARACTER INSERTED !!!!")
        
        elif param_choice == 7:
            print("source of problem matrix:")
            print("(1 - random, 2 - from file)")
            print("---Insert a number: ", end="")
            temp: int = user_input_int()
            
            if temp == 1:
                random_matrix = True
                while 1:
                    print("\nChanging randomization parameters:")
                    print(f'1. Minimal value ({min_val})\n'
                          f'2. Maximal value ({max_val})\n'
                          f'3. Number of tasks ({tasks})\n'
                          f'4. Number of machines ({machines})\n'
                          f'5. Exit\n')
                    
                    print("Your choice: ", end="")
                    matrix_choice: int = user_input_int()
                    
                    print("Changing ", end="")
                    if matrix_choice == 1:
                        print("minimal value:")
                        print("---Insert a number: ", end="")
                        min_val = user_input_int()
                    elif matrix_choice == 2:
                        print("maximal value:")
                        print("---Insert a number: ", end="")
                        max_val = user_input_int()
                    elif matrix_choice == 3:
                        print("number of tasks:")
                        print("---Insert a number: ", end="")
                        tasks = user_input_int()
                    elif matrix_choice == 4:
                        print("number of machines:")
                        print("---Insert a number: ", end="")
                        machines = user_input_int()
                    elif matrix_choice == 5:
                        break
                    else:
                        pass
                
            elif temp == 2:
                random_matrix = False
            
            else:
                pass
        
        else:
            print("Wrong character insterted!")
        
def menu()->None:
    #### INITIAL SIMULATION SETTINGS ####
    initial_cardinality: int = 200
    iterations: int = 100
    mutation_prob: float = 0.1
    selection_constant: int = 10
    random_matrix: bool = True
    ox: bool = True
    tasks: int = 9
    machines: int = 9
    min_val: float = 0
    max_val: float = 10
    compare: bool = False
    ##############################
    while 1:
        print("\n---------------------------------------")
        print('PROGRAM FOR FLOW SHOP SCHEDULING\n')
        print('SIMULATION SETTINGS')
        print(f'Initial cardinality: {initial_cardinality}\n'
              f'Number of iterations: {iterations}\n'
              f'Mutation probability: {mutation_prob}\n'
              f'Constant for tournament selection: {selection_constant}')
        print("Crossover operator: ", end="")
        if ox:
            print("ox")
        else:
            print("pmx")
        print(f'Compare crossover operators? {compare}\n')
        if random_matrix:
            print('Random problem matrix generating parameters:')
            print(f'Minimal value: {min_val}\n'
                  f'Maximal value: {max_val}\n'
                  f'Number of tasks: {tasks}\n'
                  f'Number of machines: {machines}\n')
        else:
            print('Problem matrix from file matrix.txt\n')
        
        print("MENU")
        print(f'1. Run simulation\n'
              f'2. Change simulation parameters\n'
              f'3. Exit')
        
        print("Your choice: ", end="")
        user_choice: int = user_input_int()
            
        if user_choice == 1:
            pop = Population(initial_cardinality, iterations, mutation_prob,
                     selection_constant, random_matrix, ox, tasks, machines,
                     min_val, max_val)
            run(pop, compare)
            input("Type anything to continue...")
            
        elif user_choice == 2:
            initial_cardinality, iterations, mutation_prob, selection_constant, random_matrix, ox, tasks, machines, min_val, max_val, compare = param_change(initial_cardinality, iterations,
            mutation_prob, selection_constant, random_matrix, ox, tasks,
            machines, min_val, max_val, compare)
            
        elif user_choice == 3:
            break
        
        else:
            pass

menu()

