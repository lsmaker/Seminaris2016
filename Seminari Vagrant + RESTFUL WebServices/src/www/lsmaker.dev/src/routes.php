<?php
// Routes

$app->get('/', function ($request, $response, $args) {
    // Sample log message
    $this->logger->info("Slim-Skeleton '/' route");

	$db = $this->db;

    // Render index view
    return $this->renderer->render($response, 'index.phtml', $args);
});

$app->get("/student",function($request,$response,$args){

	try{

		//Prepare the SQL statement to catch all the records on the database
		//that have not been soft-deleted previously.
		$sql = $this->db->prepare("SELECT * FROM alumnes WHERE deleted_at IS NULL");
		$sql->execute();
		$data = $sql->fetchAll(PDO::FETCH_ASSOC);

		//Return back the result list to the API caller.
		return $response->withJson([
			"status" => "OK",
			"message" => "",
			"data" => $data
		]);

	}catch(Exception $e){

		//In case that the service experiments any kind of error, return back
		//an unhandled exception message in JSON format to the API caller.
		return $response->withJson([
			"status" => "KO",
			"message" => "Internal Server Error occurred.",
			"data" => []
		])->withStatus(500);

	}
});

$app->post("/student",function($request,$response,$args){

	try{

		//Check if request has the Content-Type header assigned
		if($request->hasHeader("Content-Type")){

			//Check if the Content-Type header of the request specifies that the incoming payload is JSON formated.
			if(strtolower($request->getHeaderLine("Content-Type")) == "application/json"){

				//Parse the incoming JSON to the $body variable (associative array format)
				$body = $request->getParsedBody();

				//Check if the received payload contains the field id and its value can be
				//casted to an integer value (check filter_var method documentaton to know
				//more about that powerful function).
				if(array_key_exists("id",$body) === true &&
					filter_var($body["id"],FILTER_VALIDATE_INT) !== false){

						//Prepare the SQL statement to check if there is any record on the DB
						//that match the received id number. (If you don't know why we are using
						//prepared statemetns to build queries you should take a look to any post
						//related with SQL injections on the internet.)
						$sql = $this->db->prepare("SELECT COUNT(id) FROM alumnes WHERE id = :id");
						$sql->bindParam(":id",$body["id"],PDO::PARAM_INT);
						$sql->execute();
						$rowsCount = $sql->fetchColumn();

						//Get the number of rows that matched the query above. If
						//the number of matches is bigger than 0 it means that the
						//received id already exists on the database and we should
						//return back an error to the API user.
						if($rowsCount > 0){
							return $response->withJson([
								"status" => "KO",
								"message" => "User already exists.",
								"data" => []
							])->withStatus(500);
						}


				}else{
					//If the body field doesn't exist or contains a non-integer
					//value force it to take the null value. (This allows to
					//maintain consistence of format of all the requests received
					//independently if they are well formated or not by the API
					//caller).
					$body["id"] = null;
				}

				//Prepare the INSERT query in order to insert the new student
				//into the database. Again, we use SQL prepared statements in
				//order to avoid any chance of SQL injection.
				$sql = $this->db->prepare("INSERT INTO alumnes (id, email, created_at, updated_at, deleted_at) VALUES (:id,:email,:created_at,:updated_at,:deleted_at)");
				$sql->bindParam(":id",$body["id"],PDO::PARAM_INT);
				$sql->bindParam(":email",$body["email"]);
				$sql->bindParam(":created_at",$body["created_at"]);
				$sql->bindParam(":updated_at",$body["updated_at"]);
				$sql->bindParam(":deleted_at",$body["deleted_at"]);
				$sql->execute();

				//Retrieve back the last inserted id on the database for the
				//current connection. In this case, it will also correspond to
				//the new student inserted id value. For this reason, we collect
				//it back and assign to the id field of the received payload that
				//will be returned to the API caller.
				$body["id"] = $this->db->lastInsertId();

				//Finally, return back the payload to the caller in order to
				//make him know that the student creation has been executed
				//properly without errors.
				return $response->withJson([
					"status" => "OK",
					"message" => "User created successfully.",
					"data" => [$body]
				]);


			}

		}

		//If the code reaches this part it means that no Content-Type header
		//was present on the request or that it's value wasn't "application/json"
		//so we have to return a BadRequest error.
		return $response->withJson([
			"status" => "KO",
			"message" => "The request you performed doesn't match the expected format.",
			"data" => []
		])->withStatus(400);

	}catch(Exception $e){

		//In case that the code generates an unhandled exception catch it and
		//return back a 500 error to the API consumer. That way we avoid Slim
		//framework to display the HTTP error page that will break any client
		//that was expecting a JSON response from the server.
		return $response->withJson([
			"status" => "KO",
			"message" => "Internal Server Error.",
			"data" => []
		])->withStatus(500);

	}

});

$app->put("/student",function($request,$response,$args){

	try{

		//Check for the request required headers.
		if($request->hasHeader("Content-Type")){

			//Check if the API caller has set the Content-Type header to the
			//value the API is expecting.
			if(strtolower($request->getHeaderLine("Content-Type")) == "application/json"){

				//Parse the request JSON into PHP associative array.
				$body = $request->getParsedBody();

				//Check that the received payload contains an id and it's a valid
				//integer value.
				if(array_key_exists("id",$body) && filter_var($body["id"],FILTER_VALIDATE_INT) !== false){

					//Check that the received user exists into database.
					$sql = $this->db->prepare("SELECT count(id) FROM alumnes WHERE id = :id");
					$sql->bindParam(":id",$body["id"],PDO::PARAM_INT);
					$sql->execute();
					$rowsCount = $sql->fetchColumn();

					//Get the number of rows that matched the query above. If
					//the number of matches is equal to 1 it means that the
					//received user exists into database and can be updated.
					if($rowsCount == 1){

						//Prepare the update query.
						$sql = $this->db->prepare("UPDATE alumnes SET email = :email, created_at = :created_at, updated_at = :updated_at, deleted_at = :deleted_at WHERE id = :id");
						$sql->bindParam(":id",$body["id"],PDO::PARAM_INT);
						$sql->bindParam(":email",$body["email"]);
						$sql->bindParam(":created_at",$body["created_at"]);
						$sql->bindParam(":updated_at",$body["updated_at"]);
						$sql->bindParam(":deleted_at",$body["deleted_at"]);
						$sql->execute();

						//Return back the updated information to the API caller.
						return $response->withJson([
							"status" => "OK",
							"message" => "Entity updated successfully.",
							"data" => [$body]
						]);

					}else{

						//If no occurrences of the requested entity were found in
						//the databse, return back a NotFound response to the API
						//caller.
						return $response->withJson([
								"status" => "KO",
								"message" => "The entity you requested to update was not found in the database.",
								"data" => []
						])->withStatus(404);

					}

				}

			}

		}

		//If the code reaches this part it means that something on the request
		//or its payload is wrong so we have to return back a BadRequest response
		//to the API caller.

		return $response->withJson([
			"status" => "KO",
			"message" => "The request doesn't have the expected format.",
			"data" => []
		])->withStatus(400);

	}catch(Exception $e){

		return $response->withJson([
			"status" => "KO",
			"message" => "Internal Server Error",
			"data" => []
		])->withStatus(500);

	}

});

$app->delete("/student",function($request, $response,$args){

	try{

		if($request->hasHeader("Content-Type")){

			if(strtolower($request->getHeaderLine("Content-Type")) == "application/json"){

				//Parse the request payload into $body PHP associative array.
				$body = $request->getParsedBody();

				//Check if the request body contains the id field that identifies the
				//entity to be removed from the database.
				if(array_key_exists("id",$body) &&
					filter_var($body["id"],FILTER_VALIDATE_INT) !== false){

						//Check if the entity exists on the database.
						$sql = $this->db->prepare("SELECT count(id) FROM alumnes WHERE id = :id");
						$sql->bindParam(":id",$body["id"],PDO::PARAM_INT);
						$sql->execute();

						$rowsCount = $sql->fetchColumn();

						if($rowsCount == 1){

							//Build and execute the deletion statement.
							$sql = $this->db->prepare("DELETE FROM alumnes WHERE id = :id");
							$sql->bindParam(":id",$body["id"],PDO::PARAM_INT);
							$sql->execute();

							//Return a JSON response to the API caller notifying
							//that the entity has been sucessfully removed from the
							//datbaase.
							return $response->withJson([
								"status" => "OK",
								"message" => "The entity has been removed successfully.",
								"data" => []
							]);

						}else{

							//Return a JSON response to the API caller saying that
							//the requested entity could not be deleted because it
							//doesn't exists on the database.
							return $response->withJson([
								"status" => "KO",
								"message" => "The entity you required to be deleted was not found on the databse.",
								"data" => []
							])->withStatus(404);

						}


				}

			}

		}

		//If the code reaches this point it means that it was something wrong with
		//the request payload so we have to notify the API caller by returning a JSON
		//error response
		return $response->withJson([
			"status" => "KO",
			"message" => "The request's payload doesn't match the expected format.",
			"data" => []
		])->withStatus(400);

	}catch(Exception $e){

		//If an unexpected error happens, return back the message but in JSON
		//format in order to avoid breaking the API caller's code that is
		//expecting a JSON response from us instead of an StackTrace.
		return $response->withJson([
			"status" => "KO",
			"message" => "Internal Server Error.",
 		])->withStatus(500);

	}

});
